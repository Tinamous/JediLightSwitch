// Responsible for handling the interface with the 4 UI LEDs in the corners of the board.

 
#ifndef LedHandler_H
#define LedHandler_H

#include "neopixel/neopixel.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
class LedHandler {
public:
    LedHandler();
    ~LedHandler();
    
    virtual bool init();
    void debugLedOn();
    void debugLedOff();
    void ledOn(int led);
    void ledOff(int led);
    void allLedsOn();
    void topLedsOn();
    void bottomLedsOn();
    void leftLedsOn();
    void rightLedsOn();
    void ledsOff();
    
    virtual void glow();
    virtual void rainbowGlow();
    virtual void noglow();
    
protected:
    LedHandler(int topLeftLed, int topRightLed, int bottomLeftLed, int bottomRightLed, int debugLed);
    

private:
    int _topLeftLed;
    int _topRightLed;
    int _bottomLeftLed;
    int _bottomRightLed;
    int _debugLed;     
};

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
class LedHandlerV1 : public LedHandler {
public:
    LedHandlerV1();
};

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
class LedHandlerV2 : public LedHandler {
public:
    LedHandlerV2();
    bool init() override;
    void glow() override;
    void noglow() override;
    void rainbowGlow() override;
private:
    // V2 hardware supports NeoPixels on the back of the PCB
    void rainbow(uint8_t wait);
    uint32_t Wheel(byte WheelPos);
    Adafruit_NeoPixel _strip = Adafruit_NeoPixel(4, A7, WS2812B);
};

#endif