
#include "canflash.h"

/* main program -------------------------------------------------*/

int main(int argc, char **argv){
  /* Begin with loading file */
  char filepath[] = "./flashdata/test.png";
  uint8_t *firmwareBuffer;
  uint32_t filesize;

  loadFile(filepath, &firmwareBuffer, &filesize);

  CAN_init();

  CAN_txData(firmwareBuffer, filesize);



}


uint8_t loadFile(char *filePath, uint8_t **fileBuffer, uint32_t *fileLen){
  FILE *fileptr;
  char filename[255];
  *fileLen = 0;
  
  strcpy(filename, filePath);
  
  if(access(filename, F_OK) != -1){
    fileptr = fopen(filename, "rb");
    fseek(fileptr, 0, SEEK_END);
    *fileLen = ftell(fileptr);
    rewind(fileptr);
    
    *fileBuffer = (char *)malloc((*fileLen+1)*sizeof(char));
    fread(*fileBuffer, *fileLen, 1, fileptr);
    fclose(fileptr);
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

CANIF_TypeDef CAN_txFrame(uint16_t id, uint8_t *frameData, uint8_t dlc){
  canstat = canWrite(canhnd, id, frameData, dlc, 0x00);
  if(canstat != canOK)return CAN_statusHandler(canstat);
  return CANIF_OK;
}

CANIF_TypeDef CAN_txData(uint8_t *fileBuffer, uint32_t fileLen){
  uint32_t doubleWords = fileLen / 8;
  printf("Len:%d", fileLen);
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


    CAN_txFrame(1, &txBuffer, txLen);
  }


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