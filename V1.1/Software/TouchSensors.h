// Responsible for handling temperature and humidity measurements.

#ifndef TouchSensors_H
#define TouchSensors_H

#include "LedHandler.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
class TouchSensors {
public:
    TouchSensors(int touchInput, int touchVdd, LedHandler* leds);
    bool init();
    void checkIfTouched();
private:
    void touchInterrupt();
    void handleTouch();
    int getTouchedCount();
    void processTouch(int touchedCount);
    void doTouchedAction();
    void doLongTouchAction();
    void doVeryLongTouchAction();
    LedHandler* _leds;
    int _touchInput;
    int _touchVdd;
    volatile int _touched = false;
    // Number of counts of touched (with a 250ms delay for each)
    int _longTouchThreshold = 4;
    int _veryLongTouchThreshold = 20;
};

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
class TouchSensorsV1 : public TouchSensors {
public:
    TouchSensorsV1(LedHandler* leds);

};

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
class TouchSensorsV2 : public TouchSensors {
public:
    TouchSensorsV2(LedHandler* leds);
};

#endif