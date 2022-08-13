#ifndef _PROFILESTASK_H_
#define _PROFILESTASK_H_

#include <stdint.h>

uint32_t setup_profile(void *data, void *user_data);

#define PRINT_PORT    APB_UART0

#define PIN_UART_RX   GIO_GPIO_4

#define SD_SPI_CH	   SPI_PORT_0
#define SD_SPI_SSP     AHB_SSP0

#define SD_SPI_PIN_CS      GIO_GPIO_8
#define SD_SPI_PIN_MOSI    GIO_GPIO_9
#define SD_SPI_PIN_SCK     GIO_GPIO_10
#define SD_SPI_PIN_MISO    GIO_GPIO_11

#define INPUT_KEY_1         GIO_GPIO_1
#define INPUT_KEY_2         GIO_GPIO_5

#endif


