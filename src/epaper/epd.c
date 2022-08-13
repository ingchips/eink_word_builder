
#include "epd.h"

static int cur_model_index = 0;

typedef void (*eink_init)(UBYTE mode);
typedef void (*eink_clear)(void);
typedef void (*eink_sleep)(void);
typedef void (*eink_display)(UBYTE *Image, UBYTE *Image2);

typedef struct eink_reg
{
    int tp;
    int w;
    int h;
    eink_init init;
    eink_clear clear;
    eink_sleep sleep;
    eink_display display;
}eink_reg_t;

static const eink_reg_t eink_regs[] = {
        {.tp=MODEL_1in54,.init=EPD_1IN54_Init, .w = EPD_1IN54_WIDTH, .h = EPD_1IN54_HEIGHT, .clear = EPD_1IN54_Clear, .sleep =EPD_1IN54_Sleep, .display=EPD_1IN54_Display},
        {.tp=MODEL_1in54b,.init=EPD_1IN54B_Init, .w = EPD_1IN54B_WIDTH, .h = EPD_1IN54B_HEIGHT, .clear = EPD_1IN54B_Clear, .sleep =EPD_1IN54B_Sleep, .display=EPD_1IN54B_Display},
        {.tp=MODEL_1in54b_V2,.init=EPD_1IN54B_V2_Init, .w = EPD_1IN54B_V2_WIDTH, .h = EPD_1IN54B_V2_HEIGHT, .clear = EPD_1IN54B_V2_Clear, .sleep =EPD_1IN54B_V2_Sleep, .display=EPD_1IN54B_V2_Display},
        {.tp=MODEL_1in54c,.init=EPD_1IN54C_Init, .w = EPD_1IN54C_WIDTH, .h = EPD_1IN54C_HEIGHT, .clear = EPD_1IN54C_Clear, .sleep =EPD_1IN54C_Sleep, .display=EPD_1IN54C_Display},
        //{.tp=MODEL_1in54f,.init=EPD_1IN54FF_Init, .w = EPD_1IN54F_WIDTH, .h = EPD_1IN54F_HEIGHT, .clear = EPD_1IN54FF_Clear, .sleep =EPD_1IN54FF_Sleep, .display=EPD_1IN54FF_Display},
        {.tp=MODEL_1in54_V2,.init=EPD_1IN54_V2_Init, .w = EPD_1IN54_V2_WIDTH, .h = EPD_1IN54_V2_HEIGHT, .clear = EPD_1IN54_V2_Clear, .sleep =EPD_1IN54_V2_Sleep, .display=EPD_1IN54_V2_Display},
        {.tp=MODEL_1in54_V3,.init=EPD_1IN54_V3_Init, .w = EPD_1IN54_V3_WIDTH, .h = EPD_1IN54_V3_HEIGHT, .clear = EPD_1IN54_V3_Clear, .sleep =EPD_1IN54_V3_Sleep, .display=EPD_1IN54_V3_Display},
    {.tp = -1}
};

void EPD_Model(UBYTE model) {
    if (model >= MODEL_MAX)
        return;
    const eink_reg_t* reg = eink_regs;
    int index = 0;
    while (reg->tp >= 0) {
        if (reg->tp == model) {
            cur_model_index = index;
            break;
        }
        reg ++;
        index ++;
    }
}

int EPD_Init(UBYTE mode, size_t *w, size_t *h) {
    eink_regs[cur_model_index].init(mode);
    *w = eink_regs[cur_model_index].w;
    *h = eink_regs[cur_model_index].h;
    return 0;
}
void EPD_Clear(void) {
    eink_regs[cur_model_index].clear();
}
void EPD_Display(UBYTE *Image, UBYTE *Image2) {
    if (Image2 == NULL)
        Image2 = Image;
    eink_regs[cur_model_index].display(Image, Image2);
}
void EPD_Sleep(void) {
    eink_regs[cur_model_index].sleep();
}
