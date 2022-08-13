#pragma once

#include <stdint.h>
#include <string.h>
#include "ingsoc.h"

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

typedef struct eink_conf {
    uint8_t busy_pin;
    uint8_t res_pin;
    uint8_t dc_pin;
    uint8_t cs_pin;
    uint8_t scl_pin;
    uint8_t sda_pin;
    uint8_t full_mode;
    SSP_TypeDef *spi;
    io_source_t io_spi_clk;
    io_source_t io_spi_mosi;
    void* userdata;
}eink_conf_t;

extern eink_conf_t econf;

/**
 * e-Paper GPIO
**/
#define EPD_RST_PIN     (econf.res_pin)
#define EPD_DC_PIN      (econf.dc_pin)
#define EPD_CS_PIN      (econf.cs_pin)
#define EPD_BUSY_PIN    (econf.busy_pin)

/**
 * delay x ms
**/
void DEV_Delay_ms(int xms);

void DEV_Digital_Write(uint8_t pin, uint8_t value);
int DEV_Digital_Read(uint8_t pin);

void DEV_SPI_WriteByte(UBYTE value);

int DEV_Module_Init(void);
void DEV_Module_Exit(void);
