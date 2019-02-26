#gcc -c src/canflash.c -Iinc/ -obuild/canflash.o && gcc build/canflash.o -lz -o build/canflash

CC = gcc
LDLIBS = -lz

SRC_FILES = canflash.c

SRC_DIR = /src/
INC_DIR = /inc/
BUILD_DIR = /build/

OBJ = $(subst .c,.o,$(SRC_FILES))

all:

canflash.o:
	$(CC) $(SRC_FILES)
