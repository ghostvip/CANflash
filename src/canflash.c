
#include "canflash.h"

/* main program -------------------------------------------------*/

int main(int argc, char **argv){
  /* Begin with loading file */
  
  char filepath[] = "./flashdata/test.png";
  uint8_t *firmwareBuffer;
  uint32_t filesize;

  loadFile(filepath, &firmwareBuffer, &filesize);

  CAN_init();
  uint8_t data[4];
  data[0] = 0;
  data[1] = 1;
  data[2] = 2;
  data[3] = 3;
  CAN_txFrame(666, &data, 4);



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

CANIF_TypeDef CAN_txData(uint8_t *fileBuffer, uint32_t *fileLen){
  uint32_t doubleWords = fileLen / 8;
  uint8_t txLen;

  for(int doubleWordPtr = 0; doubleWordPtr < (doubleWords + 1); doubleWordPtr++){
    if(doubleWordPtr <= doubleWords){
      txLen = 8;
    }else{
      txLen = doubleWordsModulo = fileLen % 8;
    }

    if(CAN_txFrame(CAN_BL_FLASHID, fileBuffer[doubleWordPtr * 8], txLen) != CANIF_OK){
      return CANIF_ERROR;
    }
    
    for(int i = 0; i < txLen; i++){
      printf("%d\t", fileBuffer[doubleWordPtr * 8 + txLen]);
    }
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