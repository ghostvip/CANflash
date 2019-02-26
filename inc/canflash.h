#ifndef CANFLASH_H
#define CANFLASH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>

#include <argp.h>
#include <zlib.h>


/* file variables ----------------------------------------------*/
uint32_t filelen;
uint8_t *filebuffer;

/* fw variables ----------------------------------------------*/
uint32_t crc32_checksum;

/* function prototypes ----------------------------------------------*/
uint8_t load_file(char *filepath);
uint32_t crc32_calculate(void);

#endif /* CANFLASH_H */
