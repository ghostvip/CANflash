/*
 * ID(1), BL_CAN_FREQUEST(1)                    //*MASTER sends request to flash and waits for response while *SLAVE does reset
 * ...
 * ID(1), BL_CAN_OK(1)                          //*SLAVE confirms software reset and flash request
 * ID(1), BL_CAN_FINFO(1), SIZE(2), CRC32(4)    //*MASTER sends information about flash procedure
 * ID(1), BL_CAN_OK(1)                          //*SLAVE confirms if ready to receive
 * DATA(8)                                      //*MASTER sends all data
 * ...                                          //*MASTER timeouts for 1 second to confirm end of data
 * ID(1), BL_CAN_OK(1)                          //*SLAVE responds with: BL_CAN_OK, BL_CAN_CRCERR, BL_CAN_SIZEERR 
 */
#ifndef CANFLASH_H
#define CANFLASH_H

/*Includes --------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>

#include <canlib.h>

/* Defines -----------------------------------------------------------*/
#define CAN_CHANNEL 1
#define CAN_BL_FLASHID 0xAF
#define CAN_BAUDRATE canBITRATE_250K

/* Typedefs ---------------------------------------------------------*/
typedef enum{
    CAN_BL_OK = 0x01,
    CAN_BL_CRCERR = 0x02,
    CAN_BL_SIZEERR = 0x03,
    CAN_BL_FREQUEST = 0x04,
    CAN_BL_FINFO = 0x05
}CAN_BL_TypeDef;

typedef enum{
    CANIF_OK = 0x01,
    CANIF_ERROR = 0x02,
    CANIF_BUSY = 0x03,
    CSNIF_TIMEOUT = 0x04
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
 * @return Returns 0 if loading failes, returns 1 if loading succeded
 */
uint8_t loadFile(char *filePath, uint8_t **fileBuffer, uint32_t *fileLen);

/**
 * @brief Initializes CAN interface
 * @return Returns status as CANIF_TypeDef
 * @todo Add other interfaces as KVASER leaf light
 */
CANIF_TypeDef CAN_init(void);

/**
 * @brief Transmits data via CAN interface
 * @param[in] id ID of CAN message
 * @param[in] frameData Data to transmit via CAN
 * @param[in] dlc Data Length Code of CAN message, also length of frameData
 * @return Returns CANIF_TypDef
 */
CANIF_TypeDef CAN_txFrame(uint16_t id, uint8_t *frameData, uint8_t dlc);
CANIF_TypeDef CAN_txData(uint8_t *fileBuffer, uint32_t *fileLen);

CANIF_TypeDef CAN_statusHandler(canStatus canStatusHandler);

#endif /* CANFLASH_H */
