
#include "canflash.h"

/* main program -------------------------------------------------*/

int main(int argc, char **argv){
  /* Begin with loading file */
  char filepath[] = "./flashdata/test.png";
  uint8_t *firmwareBuffer;
  uint32_t filesize;
  uint32_t filecrc32;

  /* load firmware file into buffer */
  loadFile(filepath, &firmwareBuffer, &filesize, &filecrc32);

  /* initialize CAN interface */
  CAN_init();


  /* begin flash procedure -------------------- */

  /* request node with nodeId to perform soft reset */
  CAN_requestFlash(CAN_BL_NODEID);
  /* w ait for node with nodeId to ACK, exit if not NAK/no response */
  if(CAN_awaitACK(CAN_BL_NODEID, CAN_BL_ACKTIMEOUT) != CANIF_OK) return 0;

  /* send info about current flash procecure */
  CAN_transmitFlashInfo(CAN_BL_NODEID, filesize, filecrc32);
  /* wait for node with nodeId to ACK, exit if not NAK/no response */
  if(CAN_awaitACK(CAN_BL_NODEID, CAN_BL_ACKTIMEOUT) != CANIF_OK) return 0;


}


uint8_t loadFile(char *filePath, uint8_t **fileBuffer, uint32_t *fileLen, uint32_t *fileCrc32){
  FILE *fileptr;
  char filename[255];
  *fileLen = 0;

  /*@TODO fileCrc32 has to be calculated correctly */
  *fileCrc32 = 0xa0b1c2d3;
  
  strcpy(filename, filePath);
  
  if(access(filename, F_OK) != -1){
    fileptr = fopen(filename, "rb");
    fseek(fileptr, 0, SEEK_END);
    *fileLen = ftell(fileptr);
    rewind(fileptr);
    
    *fileBuffer = (char *)malloc((*fileLen+1)*sizeof(char));
    fread(*fileBuffer, *fileLen, 1, fileptr);
    fclose(fileptr);

#ifdef DEBUG
    printf("loaded file [%s], size: %d(0x%x), crc32: 0x%x\n", filePath, *fileLen, *fileLen, *fileCrc32);
#endif

    return 1;
  }
  return 0;
}

CANIF_TypeDef CAN_init(void){
  canInitializeLibrary();
  canhnd = canOpenChannel(CAN_CHANNEL, canOPEN_EXCLUSIVE | canOPEN_REQUIRE_EXTENDED | canOPEN_ACCEPT_VIRTUAL);
  if(canhnd < 0)return CANIF_ERROR;

  canstat = canSetBusParams(canhnd, CAN_BAUDRATE, 0, 0, 0, 0, 0);
  if(canstat != canOK)return CAN_statusHandler(canstat);

  canstat = canBusOn(canhnd);
  if(canstat != canOK)return CAN_statusHandler(canstat);

  return CANIF_OK;
}

CANIF_TypeDef CAN_txFrame(uint8_t *frameData){
  canstat = canWrite(canhnd, CAN_BL_FLASHID, frameData, CAN_BL_DLC, 0x00);
  if(canstat != canOK)return CAN_statusHandler(canstat);
  return CANIF_OK;
}

CANIF_TypeDef CAN_txData(uint8_t *fileBuffer, uint32_t fileLen){
  uint32_t doubleWords = fileLen / 8;
  uint8_t txLen;
  uint8_t txBuffer[8];

  for(int doubleWordPtr = 0; doubleWordPtr < (doubleWords + 1); doubleWordPtr++){
    if(doubleWordPtr <= doubleWords){
      txLen = 8;
    }else{
      txLen = fileLen % 8;
    }

    for(int i = 0; i < txLen; i++){
      txBuffer[i] = fileBuffer[doubleWordPtr*8 + i];
    }
    CAN_txFrame(&txBuffer);
  }
}

CANIF_TypeDef CAN_requestFlash(uint16_t nodeId){
  uint8_t txData[8];

  txData[0] = nodeId;
  txData[1] = CAN_BL_FREQUEST;
  txData[2] = 0x00;
  txData[3] = 0x00;
  txData[4] = 0x00;
  txData[5] = 0x00;
  txData[6] = 0x00;
  txData[7] = 0x00;

  CAN_txFrame(&txData);
  
#ifdef DEBUG
  printf("Requested node %d to reset into flash mode\n", nodeId);
#endif
}


CANIF_TypeDef CAN_transmitFlashInfo(uint16_t nodeId, uint16_t fileLen, uint32_t fileCrc32){
  uint8_t txData[8];

  txData[0] = nodeId;
  txData[1] = CAN_BL_FREQUEST;
  txData[2] = ((fileLen) >> 8 ) & 0xFF;
  txData[3] = (fileLen) & 0xFF;
  txData[4] = ((fileCrc32) >> (8*3)) & 0xFF;
  txData[5] = ((fileCrc32) >> (8*2)) & 0xFF;
  txData[6] = ((fileCrc32) >> (8*1)) & 0xFF;
  txData[7] = (fileCrc32) & 0xFF;

  CAN_txFrame(&txData);

#ifdef DEBUG
  printf("Transmitted information about flash procedure to node %d\n", nodeId);
#endif
}

CANIF_TypeDef CAN_awaitACK(uint8_t nodeId, uint16_t timeout){
  uint8_t rxData[8];
  uint8_t rxID = CAN_BL_FLASHID;
  uint8_t rxDLC;
  uint8_t rxFlag;
  uint64_t rxTime;

  /*@TODO waiting for specific id with timeout has to be implemented*/

  canstat = canReadSpecific(canhnd, rxID, &rxData, &rxDLC, &rxFlag, &rxTime);
  if(canstat != canOK){
    printf("No ACK was received: ");
    return CAN_statusHandler(canstat);
  }

  return CANIF_OK;
}

CANIF_TypeDef CAN_statusHandler(canStatus canStatusHandler){
  if(canStatusHandler != canOK){
    char textBuffer[255];
    textBuffer[0] = 0x00;
    canGetErrorText(canStatusHandler, textBuffer, sizeof(textBuffer));
    printf("Error %d (%s)\n", (int)canStatusHandler, textBuffer);
    return CANIF_ERROR;
  }
  return CANIF_OK;
}
