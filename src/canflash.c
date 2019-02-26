
#include "canflash.h"

/* argp variables ----------------------------------------------*/
const char *argp_program_version = "argp-ex2 1.0";
const char *argp_program_address = "<info@strahler.io>";
static char doc[] = "CLI for flashing user data to microcontroller using CANload.";
static char args_doc[] = "ARGLEL LLLOL";

static struct argp argp = {0, 0, 0, doc};


/* main program ---------------------------------------------*/

int main(int argc, char **argv){
  argp_parse(&argp, argc, argv, 0, 0, 0);
}





/**
 * @brief Function loads file
 * @retval 0 for positive file load - 1 for error while loading file
 */

uint8_t load_file(char *filepath){
  FILE *fileptr;
  char filename[255];
  
  strcpy(filename, filepath);
  
  if(access(filename, F_OK) != -1){
    fileptr = fopen(filename, "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);
    
    filebuffer = (char *)malloc((filelen+1)*sizeof(char));
    fread(filebuffer, filelen, 1, fileptr);
    fclose(fileptr);
    return 0;
  }
  return 1;
}





/**
 * @brief Caculates crc sum of filebuffer
 * 
 * 
 */

uint32_t crc32_calculate(void /*TODO filebuffer and filelen*/){
  uint32_t crc32_temp = crc32(0L, Z_NULL, 0);

  crc32_temp = crc32(crc32_temp, filebuffer, filelen);
  
  return crc32_temp;
}
