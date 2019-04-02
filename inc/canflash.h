/* 0    1   2   3   4   5   6   7 
 * ID(1), BL_CAN_FREQUEST(1)                    //*MASTER sends request to flash and waits for response while *SLAVE does reset
 * ...
 * ID(1), BL_CAN_ACK(1)/NAK(1)                  //*SLAVE confirms software reset and flash request
 * ID(1), BL_CAN_FINFO(1), SIZE(2), CRC32(4)    //*MASTER sends information about flash procedure
 * ID(1), BL_CAN_ACK(1)/NAK(1)                  //*SLAVE confirms if ready to receive
 * DATA(8)                                      //*MASTER sends all data
 * ...                                          //*MASTER timeouts for 1 second to confirm end of data
 * ID(1), BL_CAN_ACK(1)/NAK(1)                  //*SLAVE responds with: BL_CAN_OK, BL_CAN_CRCERR, BL_CAN_SIZEERR 
 */
#ifndef CANFLASH_H
#define CANFLASH_H

/*Includes --------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>
#include <sys/time.h>

#include <unistd.h>

#include <canlib.h>

/* Defines -----------------------------------------------------------*/
#define CAN_CHANNEL 1
#define CAN_BAUDRATE canBITRATE_250K

/* Hardware Defines*/
#define CAN_BL_FLASHID 0xAF
#define CAN_BL_DLC 0x8
#define CAN_BL_NODEID 3

#define CAN_BL_ACKTIMEOUT 20000

/* Typedefs ---------------------------------------------------------*/
typedef enum{
    /**
     * @brief Enum to describe the CAN bootloader codes
    */
    CAN_BL_ACK = 0x01,          /**< OK/ACK */
    CAN_BL_NAK = 0x02,          /**< Not OK/NAK*/

    CAN_BL_ERRCRC= 0x11,       /**< Error in CRC checksum */
    CAN_BL_ERRSIZE = 0x12,      /**< Error in file size while transmitting or receiving */

    CAN_BL_FREQUEST = 0x21,     /**< Request begin of flash process */
    CAN_BL_FINFO = 0x22         /**< Begin to transmit flash info */
}CAN_BL_TypeDef;

typedef enum{
    /**
     * @brief Function typeDefs compatible to HAL_TypeDef
    */
    CANIF_OK = 0x01,
    CANIF_ERROR = 0x02,
    CANIF_BUSY = 0x03,
    CANIF_TIMEOUT = 0x04
}CANIF_TypeDef;

/* global variables -------------------------------------------------*/
/**
 * @brief Global handle to use interface with
 */
canHandle canhnd;
/**
 * @brief Global status for reading can stati
 */
canStatus canstat;
/* function prototypes ----------------------------------------------*/

/**
 * @brief Loads file from filesystem into given buffer
 * @param[in] filePath Pointer to string  with path of file to load into buffer
 * @param[out] fileBuffer Pointer to buffer to load file to
 * @param[out] fileLen Pointer to uint32_t for size of loaded file
 * @param[out] fileCrc32 Pointer to uint32_t for CRC32 checksum to be calculated
 * @return Returns 0 if loading failes, returns 1 if loading succeded
 */
uint8_t loadFile(char *filePath, uint8_t **fileBuffer, uint32_t *fileLen, uint32_t *fileCrc32);

/**
 * @brief Initializes CAN interface
 * @return Returns status as CANIF_TypeDef
 * @todo Add other interfaces as KVASER leaf light
 */
CANIF_TypeDef CAN_init(void);

/**
 * @brief Transmits data via CAN interface
 * @param[in] frameData Pointer to frame to transmit via CAN
 * @return Returns CANIF_TypDef
 */
CANIF_TypeDef CAN_txFrame(uint8_t *frameData);

/**
 * @brief Transmits flash data via CAN interface
 * @param[in] fileBuffer Pointer to file to transmit via CAN
 * @param[in] fileLen Pointer to uint32_t for size of file to transmit
 */
CANIF_TypeDef CAN_txData(uint8_t *fileBuffer, uint32_t fileLen);

CANIF_TypeDef CAN_statusHandler(canStatus canStatusHandler);

/**
 * @brief Requests flash process to start, microcontrollers get to perform soft reset
 * @param[in] nodeId ID of node to be flashed
*/
CANIF_TypeDef CAN_requestFlash(uint16_t nodeId);


/**
 * @brief Transmits information about flash data to node
*/
CANIF_TypeDef CAN_transmitFlashInfo(uint16_t nodeId, uint16_t fileLen, uint32_t fileCrc32);

/**
 * @brief Awaits to receive ACK message from node
 * @param[in] nodeId ID of node to be adressed
 * @param[in] timeout Timeout to wait for ACK in milliseconds
*/
CANIF_TypeDef CAN_awaitACK(uint8_t nodeId, uint16_t timeout);

/* utility functions -------------------------------------------------*/
/**
 * @brief Returns delta between two time in ms as uint64_t
 * @param[in] t2 Time do substract form vase time. Param has to be in form of "struct timeval" (/sys/time.h)
 * @param[in] t1 Base time to calculate delta to. Param has to be in form of "struct timeval" (/sys/time.h)
*/
uint64_t timeDelta(struct timeval t1, struct timeval t2);

#endif /* CANFLASH_H */
