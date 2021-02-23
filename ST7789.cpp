/*****************************************************************
 * Copyright (c) 2021 CodeAbove.
 * All rights reserved.
 *
 * This software component is licensed by CodeAbove under
 * BSD 3-Clause license, the 'License'. You may not use
 * this file except in compliance with the License. You
 * may obtain a copy of the License at:
 *                  opensource.org/licenses/BSD-3-Clause
 ****************************************************************/
#include "ST7789.h"

ST7789::ST7789(IPin& rstPin, IPin& dcPin)
: resetPin(rstPin), dataCommandPin(dcPin)
{

}

void ST7789::Reset(void)
{
    resetPin.Reset();
    Wait(10);
    resetPin.Set();
    Wait(120);
}
