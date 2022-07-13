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
#include "st7789.h"

#include <type_traits>
#include <cstring>

template <typename E>
constexpr auto etoi(E const value) {
    return static_cast<std::underlying_type_t<E>>(value);
}

template <uint16_t width, uint16_t height>
ST7789<width, height>::ST7789(IST7789Spi& spi, IST7789Pin& rstPin, IST7789Pin& dcPin)
: spi(spi), resetPin(rstPin), dataCommandPin(dcPin) {}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::SetPixel(int16_t x, int16_t y, uint16_t color) {
    WriteCommand(Command::ColAddrSet);
    WriteData(x >> 8);
    WriteData(x);
    WriteData((x+1) >> 8);
    WriteData( x+1);
    WriteCommand(Command::RowAddrSet);
    WriteData(y >> 8);
    WriteData(y);
    WriteData((y+1) >> 8);
    WriteData( y+1);
    WriteCommand(Command::MemWrite);
    WriteData(color >> 8);
    WriteData(color);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::Init() {
    Reset();
    SoftwareReset();
    SleepOut();
    SetColorMode();
    InversionOn();
    DisplayOn();
    SendScreenSize();
    memset(buffer, 0, width * height * 2);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::Refresh(void) {
    SendScreenSize();
    WriteCommand(Command::MemWrite);
    WriteData((uint8_t*)buffer, width*height*2);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::Reset(void) {
    resetPin.Reset();
    Wait(MinimumResetPulseTime_ms);
    resetPin.Set();
    Wait(MaximumBlankingTime_ms);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::SoftwareReset(void) {
    WriteCommand(Command::SwRst);
    Wait(120);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::SleepOut(void) {
    WriteCommand(Command::SleepOut);
    Wait(120);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::InversionOn(void) {
    WriteCommand(Command::InversionOn);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::DisplayOn(void) {
    WriteCommand(Command::DisplayOn);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::SetColorMode(void) {
    WriteCommand(Command::IfPixFmt);
    WriteData(0x55);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::SendScreenSize() {
    WriteCommand(Command::ColAddrSet);
    WriteData(0x00);
    WriteData(0x00);
    WriteData((width-1) >> 8);
    WriteData(width-1);
    WriteCommand(Command::RowAddrSet);
    WriteData(0x00);
    WriteData(0x00);
    WriteData((height-1) >> 8);
    WriteData((height-1)&255);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::SendTestData(void) {
    uint16_t RGB565 = 0x07E0;
    uint16_t RGB565_2 = 0xE463;
    uint16_t RGB565_3  = 0xF800;

    WriteCommand(Command::MemWrite);

    for(uint32_t i = 0u; i < width*height; i++) {
        WriteData(RGB565 >> 8);
        WriteData(RGB565);
    }

    WriteCommand(Command::MemWrite);

    for(uint32_t i = 0u; i < width*height; i++) {
        WriteData(RGB565_2 >> 8);
        WriteData(RGB565_2);
    }

    WriteCommand(Command::MemWrite);
    for(uint32_t i = 0u; i < width*height; i++) {
        WriteData(RGB565_3 >> 8);
        WriteData(RGB565_3);
    }
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::WriteData(uint8_t data) {
    dataCommandPin.Set();
    spi.Write(data);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::WriteData(const uint8_t* buf, size_t size) {
    dataCommandPin.Set();
    spi.Write(buf, size);
}

template <uint16_t width, uint16_t height>
void ST7789<width,height>::WriteCommand(Command command) {
    dataCommandPin.Reset();
    spi.Write(etoi(command));
}

template class ST7789<240, 320>;
