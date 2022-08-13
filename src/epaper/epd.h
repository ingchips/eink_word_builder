
#ifndef EPD_EPD
#define EPD_EPD

#include "stdio.h"
#include "Debug.h"
#include "Dev_Config.h"

void EPD_Model(UBYTE model);
int EPD_Init(UBYTE Mode, size_t *w, size_t *h);
void EPD_Clear(void);
void EPD_Display(UBYTE *Image, UBYTE *Image2);
void EPD_Sleep(void);

enum EPD_MODEL {
        MODEL_1in02d = 1,
        MODEL_1in54,
        MODEL_1in54b,
        MODEL_1in54b_V2,
        MODEL_1in54c,
        MODEL_1in54f,
        MODEL_1in54_V2,
        MODEL_1in54_V3,

        MODEL_MAX
};

#include "EPD_1in54.h"
#include "EPD_1in54b.h"
#include "EPD_1in54b_V2.h"
#include "EPD_1in54c.h"
#include "EPD_1in54f.h"
#include "EPD_1in54_V2.h"
#include "EPD_1in54_V3.h"

#endif
