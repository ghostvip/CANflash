#gcc -c src/canflash.c -Iinc/ -o build/canflash.o

#gcc build/canflash.o -lcanlib -o build/canflash

CC = gcc
TARGET = CANflash

DOC_DIR = ./doc
BUILD_DIR = ./build
SRC_DIR = ./src
INC_DIR = ./inc
CLIBS = -lcanlib
LDFLAGS = -I$(INC_DIR)

all: $(TARGET) doc

$(TARGET): canflash.o
	$(CC) $(BUILD_DIR)/canflash.o $(CLIBS) -o $(BUILD_DIR)/$(TARGET)

canflash.o:
	$(CC) -c $(SRC_DIR)/canflash.c -o $(BUILD_DIR)/canflash.o

.PHONY: doc

doc:
	doxygen $(DOC_DIR)/$(TARGET)

	