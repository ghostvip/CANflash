# CANflash
This is a simple command line interface for flashing microcontrollers using my CANload bootloader. Special thanks to Kvaser for supporting this project. 

## Getting Started
This informations will help you getting started flashing your microcontrollers with the CANflash tool via CAN.

### Limitaions
Currently this project is working only with the Kvaser CANlib SDK under linux. An efford is made, to make this project compatible to other CAN interfaces and beeing used with other operating systems.

Since this project does only support the CANload bootloader written by myself, this project does only work with STM32F1xx and STM32F3xx microcontrollers. The next stage is to support NXPs LPC1100 Series.

### Prerequisites
This project uses the Kvaser CANlib SDK to interface with the CAN hardware layer. Instructions on how to install the Kvaser CANlib SDK are available on the Kvaser website: https://www.kvaser.com/developer/canlib-sdk/
