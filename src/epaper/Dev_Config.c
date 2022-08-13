#include "Dev_Config.h"

#include "ingsoc.h"
#include "peripheral_ssp.h"

#include "FreeRTOS.h"
#include "task.h"

static void SPI_Init(SSP_TypeDef *SSP_Ptr)
{
    apSSP_sDeviceControlBlock param;

    apSSP_Initialize(SSP_Ptr);

    /* Set Device Parameters */
    param.ClockRate        = 1;  // sspclkout = sspclk/(ClockPrescale*(ClockRate+1))
    param.ClockPrescale    = 2;  // Must be an even number from 2 to 254
    param.eSCLKPhase       = apSSP_SCLKPHASE_TRAILINGEDGE;
    param.eSCLKPolarity    = apSSP_SCLKPOLARITY_IDLELOW;
    param.eFrameFormat     = apSSP_FRAMEFORMAT_MOTOROLASPI;
    param.eDataSize        = apSSP_DATASIZE_8BITS;
    param.eLoopBackMode    = apSSP_LOOPBACKOFF;
    param.eMasterSlaveMode = apSSP_MASTER;
    param.eSlaveOutput     = apSSP_SLAVEOUTPUTDISABLED;
    apSSP_DeviceParametersSet(SSP_Ptr, &param);
}

void DEV_SPI_WriteByte(UBYTE value)
{
    apSSP_WriteFIFO(econf.spi, value);
    apSSP_DeviceEnable(econf.spi);
    while(apSSP_DeviceBusyGet(econf.spi));
    apSSP_DeviceDisable(econf.spi);
    apSSP_ReadFIFO(econf.spi);
}

int DEV_Module_Init(void)
{
    PINCTRL_SetPadMux(econf.scl_pin, econf.io_spi_clk);
    PINCTRL_SetPadMux(econf.sda_pin, econf.io_spi_mosi);
    PINCTRL_SetPadMux(econf.cs_pin, IO_SOURCE_GPIO);
    PINCTRL_SetPadMux(econf.dc_pin, IO_SOURCE_GPIO);
    PINCTRL_SetPadMux(econf.res_pin, IO_SOURCE_GPIO);
    PINCTRL_SetPadMux(econf.busy_pin, IO_SOURCE_GPIO);

    GIO_SetDirection(econf.cs_pin, GIO_DIR_OUTPUT);
    GIO_SetDirection(econf.dc_pin, GIO_DIR_OUTPUT);
    GIO_SetDirection(econf.res_pin, GIO_DIR_OUTPUT);
    GIO_SetDirection(econf.busy_pin, GIO_DIR_INPUT);

    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_Digital_Write(EPD_RST_PIN, 1);

    SPI_Init(econf.spi);

	return 0;
}

void DEV_Module_Exit(void)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);

    //close 5V
    DEV_Digital_Write(EPD_RST_PIN, 0);
}

void DEV_Digital_Write(uint8_t pin, uint8_t value)
{
    GIO_WriteValue((GIO_Index_t)pin, value);
}

int DEV_Digital_Read(uint8_t pin)
{
    return GIO_ReadValue((GIO_Index_t)pin);
}

void DEV_Delay_ms(int xms)
{
    vTaskDelay(pdMS_TO_TICKS(xms));
}
