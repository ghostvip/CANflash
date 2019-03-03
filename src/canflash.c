
#include "canflash.h"

/* main program ---------------------------------------------*/

int main(int argc, char **argv){
  /* Begin with loading file */
  
  char filepath[] = "./flashdata/test.png";
  uint8_t *firmwareBuffer;
  uint32_t filesize;

  loadFile(filepath, &firmwareBuffer, &filesize);



}



/**
 * @brief Function loads file
 * @retval 1 at success 0 at fail
 */

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

uint8_t CAN_txFrame(uint8_t *transmitFrame){

}