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
#ifndef ST7789_H
#define ST7789_H

#include <stdint.h>
#include <stdlib.h>

class IST7789Pin {
public:
    virtual void Set(void) = 0;
    virtual void Reset(void) = 0;
};

class IST7789Spi {
public:
	virtual void Write(const uint8_t byte) = 0;
	virtual void Write(const uint8_t* buffer, size_t size) {
		for(size_t i = 0; i < size; i++) {
			Write(buffer[i]);
		}
	}
};

class ST7789 {
public:
    ST7789(IST7789Spi& spi, IST7789Pin& rstPin, IST7789Pin& dcPin, uint16_t width, uint16_t height, uint8_t* buf);

    static void Task1ms(void);

    void Init(void);
    void RefreshDisplay(void);
    void SetPixel(int16_t x, int16_t y, uint16_t color);
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }
    uint16_t* getBuffer() { return (uint16_t*)buffer; }
private:
    enum class Command : uint8_t {
        SwRst           = 0x01,
        SleepOut        = 0x11,
        InversionOn     = 0x21,
        DisplayOn       = 0x29,
        ColAddrSet      = 0x2A,
        RowAddrSet      = 0x2B,
        MemWrite        = 0x2C,
        IfPixFmt        = 0x3A,
        WriteMemCont    = 0x3C,
    };

    static constexpr uint8_t MinimumResetPulseTime_ms { 10u };
    static constexpr uint8_t MaximumBlankingTime_ms { 120u };

    IST7789Spi& spi;
    IST7789Pin& resetPin;
    IST7789Pin& dataCommandPin;
    uint16_t width, height;
    uint8_t* buffer;

    volatile static uint32_t Timer_ms;

    void Reset(void);
    void SoftwareReset(void);
    void SleepOut(void);
    void InversionOn(void);
    void DisplayOn(void);
    void SetWindow(void);
    void SetColorMode(void);
    void SetScreenSize(uint16_t height, uint16_t width);
    virtual void Wait(uint32_t ms) = 0;

    void SendTestData(void);

    void WriteData(uint8_t data);
    void WriteData(const uint8_t* data, size_t size);
    void WriteCommand(Command command);

};

#endif // ST7789_H
