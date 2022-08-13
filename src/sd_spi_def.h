#pragma once

#include "ingsoc.h"
#include "peripheral_ssp.h"

#include "profile.h"

#define CS_HIGH()	    GIO_WriteValue(SD_SPI_PIN_CS, 1)
#define CS_LOW()	    GIO_WriteValue(SD_SPI_PIN_CS, 0)
#define MMC_CD          1   /* Card detect (yes:true, no:false, default:true) */
#define MMC_WP          0   /* Write protected (yes:true, no:false, default:false) */

#define SPI_SSP         SD_SPI_SSP
#define SPI_CH          SD_SPI_CH

#define SPI_PIN_CS      SD_SPI_PIN_CS
#define SPI_PIN_MOSI    SD_SPI_PIN_MOSI
#define SPI_PIN_SCK     SD_SPI_PIN_SCK
#define SPI_PIN_MISO    SD_SPI_PIN_MISO
