#ifndef CANFLASH_H
#define CANFLASH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>

#include <canlib.h>


/* global variables -------------------------------------------------*/
/* function prototypes ----------------------------------------------*/


uint8_t loadFile(char *filePath, uint8_t **fileBuffer, uint32_t *fileLen);
#endif /* CANFLASH_H */
