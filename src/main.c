#include <stdio.h>
#include <string.h>
#include "profile.h"
#include "ingsoc.h"
#include "platform_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "trace.h"

#include "epaper/Dev_Config.h"
#include "epaper/epd.h"

#include "../fatfs/ff.h"

eink_conf_t econf =
{
    .busy_pin = 12,
    .res_pin = 17,
    .dc_pin = 16,
    .cs_pin = 13,
    .scl_pin = 19,
    .sda_pin = 18,
    .full_mode = 0,
    .spi = APB_SSP1,
    .io_spi_clk = IO_SOURCE_SPI1_CLK,
    .io_spi_mosi = IO_SOURCE_SPI1_DO,
    .userdata = NULL,
};

static uint32_t cb_hard_fault(hard_fault_info_t *info, void *_)
{
    platform_printf("HARDFAULT:\nPC : 0x%08X\nLR : 0x%08X\nPSR: 0x%08X\n"
                    "R0 : 0x%08X\nR1 : 0x%08X\nR2 : 0x%08X\nP3 : 0x%08X\n"
                    "R12: 0x%08X\n",
                    info->pc, info->lr, info->psr,
                    info->r0, info->r1, info->r2, info->r3, info->r12);
    for (;;);
}

static uint32_t cb_assertion(assertion_info_t *info, void *_)
{
    platform_printf("[ASSERTION] @ %s:%d\n",
                    info->file_name,
                    info->line_no);
    for (;;);
}

static uint32_t cb_heap_out_of_mem(uint32_t tag, void *_)
{
    platform_printf("[OOM] @ %d\n", tag);
    for (;;);
}

#define PRINT_PORT    APB_UART0

uint32_t cb_putc(char *c, void *dummy)
{
    while (apUART_Check_TXFIFO_FULL(PRINT_PORT) == 1);
    UART_SendData(PRINT_PORT, (uint8_t)*c);
    return 0;
}

int fputc(int ch, FILE *f)
{
    cb_putc((char *)&ch, NULL);
    return ch;
}

void config_uart(uint32_t freq, uint32_t baud)
{
    UART_sStateStruct config;

    config.word_length       = UART_WLEN_8_BITS;
    config.parity            = UART_PARITY_NOT_CHECK;
    config.fifo_enable       = 1;
    config.two_stop_bits     = 0;
    config.receive_en        = 1;
    config.transmit_en       = 1;
    config.UART_en           = 1;
    config.cts_en            = 0;
    config.rts_en            = 0;
    config.rxfifo_waterlevel = 1;
    config.txfifo_waterlevel = 1;
    config.ClockFrequency    = freq;
    config.BaudRate          = baud;

    apUART_Initialize(PRINT_PORT, &config, 0);
}

static uint32_t ms_timer_isr(void *user_data)
{
    extern void disk_timerproc (void);

    TMR_IntClr(APB_TMR1);
    disk_timerproc();
    return 0;
}

void setup_peripherals(void)
{
    SYSCTRL_ClearClkGateMulti(0
                            | (1 << SYSCTRL_ClkGate_APB_TMR0)
                            | (1 << SYSCTRL_ClkGate_APB_TMR1)
                            | (1 << SYSCTRL_ClkGate_APB_GPIO0)
                            | (1 << SYSCTRL_ClkGate_AHB_SPI0)
                            | (1 << SYSCTRL_ClkGate_APB_SPI1)
                            | (1 << SYSCTRL_ClkGate_APB_PinCtrl));
    config_uart(OSC_CLK_FREQ, 115200);

    platform_set_irq_callback(PLATFORM_CB_IRQ_TIMER1, ms_timer_isr, NULL);

#if (INGCHIPS_FAMILY == INGCHIPS_FAMILY_918)
    TMR_SetCMP(APB_TMR1, TMR_CLK_FREQ / 1000);
    TMR_SetOpMode(APB_TMR1, TMR_CTL_OP_MODE_WRAPPING);
    TMR_Reload(APB_TMR1);
    TMR_IntEnable(APB_TMR1);
    TMR_Enable(APB_TMR1);
#elif (INGCHIPS_FAMILY == INGCHIPS_FAMILY_916)
    #error WIP
#else
    #error unknown or unsupported chip family
#endif

    PINCTRL_SetPadMux(INPUT_KEY_1, IO_SOURCE_GENERAL);
    GIO_SetDirection(INPUT_KEY_1, GIO_DIR_INPUT);
    PINCTRL_Pull(INPUT_KEY_1, PINCTRL_PULL_UP);

    PINCTRL_SetPadMux(INPUT_KEY_2, IO_SOURCE_GENERAL);
    GIO_SetDirection(INPUT_KEY_2, GIO_DIR_INPUT);
    PINCTRL_Pull(INPUT_KEY_2, PINCTRL_PULL_UP);

    DEV_Module_Init();

    if (!IS_DEBUGGER_ATTACHED())
    {
        // Watchdog will timeout after ~20sec
        TMR_WatchDogEnable(TMR_CLK_FREQ * 10);
    }
}

uint32_t on_deep_sleep_wakeup(void *dummy, void *user_data)
{
    (void)(dummy);
    (void)(user_data);
    setup_peripherals();
    return 0;
}

uint32_t query_deep_sleep_allowed(void *dummy, void *user_data)
{
    (void)(dummy);
    (void)(user_data);
    // TODO: return 0 if deep sleep is not allowed now; else deep sleep is allowed
    return 0;
}

static void watchdog_task(void *pdata)
{
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(9000));
        TMR_WatchDogRestart();
    }
}

static void main_task(void *pdata);

trace_rtt_t trace_ctx = {0};

int app_main()
{
    platform_set_evt_callback(PLATFORM_CB_EVT_PROFILE_INIT, setup_profile, NULL);

    // setup handlers
    platform_set_evt_callback(PLATFORM_CB_EVT_HARD_FAULT, (f_platform_evt_cb)cb_hard_fault, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_ASSERTION, (f_platform_evt_cb)cb_assertion, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_HEAP_OOM, (f_platform_evt_cb)cb_heap_out_of_mem, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_ON_DEEP_SLEEP_WAKEUP, on_deep_sleep_wakeup, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_QUERY_DEEP_SLEEP_ALLOWED, query_deep_sleep_allowed, NULL);
    platform_set_evt_callback(PLATFORM_CB_EVT_PUTC, (f_platform_evt_cb)cb_putc, NULL);

    platform_config(PLATFORM_CFG_RTOS_ENH_TICK, 1);

    setup_peripherals();
    xTaskCreate(watchdog_task,
           "w",
           configMINIMAL_STACK_SIZE,
           NULL,
           (configMAX_PRIORITIES - 1),
           NULL);

    xTaskCreate(main_task,
           "d",
           1000,
           NULL,
           5,
           NULL);

    trace_rtt_init(&trace_ctx);
    platform_set_evt_callback(PLATFORM_CB_EVT_TRACE, (f_platform_evt_cb)cb_trace_rtt, &trace_ctx);
    // TODO: config trace mask
    platform_config(PLATFORM_CFG_TRACE_MASK, 0);
    return 0;
}

static uint8_t image[5000] =
{
#include "data.h"
}
;

static FATFS fs = {0};
static FIL f_q;
static FIL f_a;

#define IMAGE_BYTE_SIZE     (200 * 200 / 8)

static uint8_t g_image[IMAGE_BYTE_SIZE];

#define fatal_err(tag, code) do { platform_printf(tag " err: %d\n", code); error_fun(); } while (0)

static void error_fun(void)
{
    while (1) vTaskDelay(1000);
}

static int load_file(const char *fn, uint8_t *data, int to_read)
{
    static FIL f;
    int res = f_open(&f, fn, FA_OPEN_EXISTING | FA_READ);
    if (res)
        return 0;
    UINT read;
    f_read(&f, data, (UINT)to_read, &read);
    f_close(&f);
    return (int)read;
}

static void load_state(uint8_t *prob, int wc)
{
    load_file("state", prob, wc);
}

static void save_state(uint8_t *prob, int wc)
{
    static FIL f;
    int res = f_open(&f, "state", FA_CREATE_NEW | FA_OPEN_EXISTING | FA_WRITE);
    if (res)
        return;
    UINT write;
    f_write(&f, prob, (UINT)wc, &write);
    f_close(&f);
}

static int load_block(FIL *f, int offset, uint8_t * out, int size)
{
    int res = f_lseek(f, offset);
    if (res)
        fatal_err("lseek", res);
    UINT read;
    res = f_read(f, out, (UINT)size, &read);
    if (res)
        fatal_err("read", res);
    return (int)read;
}

#define WAIT_EINK   vTaskDelay(pdMS_TO_TICKS(1000))

static void show_start_img(void)
{
    if (load_file("start.img", g_image, IMAGE_BYTE_SIZE) != IMAGE_BYTE_SIZE)
        fatal_err("load start.img", 0);
    EPD_Display(g_image, NULL);
    WAIT_EINK;
}

static void complete(void)
{
    if (load_file("done.img", g_image, IMAGE_BYTE_SIZE) != IMAGE_BYTE_SIZE)
        fatal_err("load done.img", 0);
    EPD_Display(g_image, NULL);
    while (1) vTaskDelay(1000);
}

static void show_question_img(int n)
{
    if (load_block(&f_q, n * IMAGE_BYTE_SIZE, g_image, IMAGE_BYTE_SIZE) != IMAGE_BYTE_SIZE)
        fatal_err("show questtion", 0);
    EPD_Display(g_image, NULL);
    WAIT_EINK;
}

static void show_answer_img(int n)
{
    if (load_block(&f_a, n * IMAGE_BYTE_SIZE, g_image, IMAGE_BYTE_SIZE) != IMAGE_BYTE_SIZE)
        fatal_err("show answer", 0);
    EPD_Display(g_image, NULL);
    WAIT_EINK;
}

int detect_key(void)
{
    while (1)
    {
        vTaskDelay(30);
        if (GIO_ReadValue(INPUT_KEY_1) == 0)
        {
            do { vTaskDelay(30); } while (GIO_ReadValue(INPUT_KEY_1) == 0);
            return 1;
        }

        if (GIO_ReadValue(INPUT_KEY_2) == 0)
        {
            do { vTaskDelay(30); } while (GIO_ReadValue(INPUT_KEY_2) == 0);
            return 2;
        }
    }
}

int select_word(uint8_t *prob, int wc)
{
    uint32_t total = 0;
    int i;
    for (i = 0; i < wc; i++)
        total += prob[i];

    if (total < 1) return -1;

    uint32_t r = rand() % total;
    for (i = 0; i < wc; i++)
    {
        if (r < prob[i])
            return i;
        r -= prob[i];
    }

    fatal_err("select word", 0);
    return -1;
}

static void main_task(void *pdata)
{
    size_t w, h;
    int res = f_mount(&fs, "", 1);
    if (res)
        fatal_err("f_mount", res);

    res = f_open(&f_q, "Q.img", FA_OPEN_EXISTING | FA_READ);
    if (res)
        fatal_err("f_open(Q)", res);

    res = f_open(&f_a, "A.img", FA_OPEN_EXISTING | FA_READ);
    if (res)
        fatal_err("f_open(A)", res);

    int wc = f_size(&f_a) / IMAGE_BYTE_SIZE;

    uint8_t *prob = pvPortMalloc(wc * sizeof(uint8_t));
    memset(prob, 0xff, wc * sizeof(uint8_t));

    load_state(prob, wc);

    EPD_Model(MODEL_1in54);
    EPD_Init(0, &w, &h);
    platform_printf("size: %d x %d\n", w, h);
    show_start_img();

    detect_key();

    int num = 0;
    for (;;)
    {
        num++;
        if (num > 10)
        {
            save_state(prob, wc);
            num = 0;
        }

        int k = select_word(prob, wc);
        if (k < 0)
            complete();
        show_question_img(k);
        switch (detect_key())
        {
        case 1:
            prob[k] = prob[k] >> 2;
            break;
        case 2:
            {
                uint32_t n = prob[k] << 1;
                if (n > 0xff)
                    n = 0xff;
                prob[k] = (uint8_t)n;
            }
            show_answer_img(k);
            detect_key();
            break;
        }
    }
}

