// Responsible for handling temperature and humidity measurements.

 #include "application.h"
 #include "TouchSensors.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
// Ctor.
TouchSensors::TouchSensors(int touchInput, int touchVdd, LedHandler* leds) {
    _touchInput = touchInput;
    _touchVdd =  touchVdd;
     _leds = leds;
}

bool TouchSensors::init() {
   
   // IO Pin setup for touch in setupIO.
   // Touch
    pinMode(_touchInput, INPUT);
    pinMode(_touchVdd, OUTPUT);
    
    // Enable touch controller (1.1+ hardware)
    digitalWrite(_touchVdd, HIGH);
    
    // Attach interrupt to touchInput...
    auto interruptHandler = std::bind(&TouchSensors::touchInterrupt, this);
    attachInterrupt(_touchInput, interruptHandler, RISING);

    return true;
}

void TouchSensors::checkIfTouched() {
    if (_touched) {
        handleTouch();
    }
}

void TouchSensors::handleTouch() {
    noInterrupts();
        
    // Initial indiation that touch has been sensed.
    _leds->allLedsOn();
    
    int touchedCount = getTouchedCount();
    
    processTouch(touchedCount);
    
    delay(200);
    _leds->ledsOff();
    
    _touched = false;
    interrupts();
}

int TouchSensors::getTouchedCount() {
    int touchedCount = 0;
    bool touched;
    
    do {
        touched = digitalRead(_touchInput);
        
        if (touched) {
            touchedCount++;
            delay(250);
            
            // If long touch then flash left/right
            // leds to show long touch accepted
            if (touchedCount > _veryLongTouchThreshold) {
                _leds->ledsOff();
                if (touchedCount % 2 == 0) {
                    _leds->topLedsOn();
                } else {
                    _leds->bottomLedsOn();
                }
            } else if (touchedCount > _longTouchThreshold) {
                _leds->ledsOff();
                if (touchedCount % 2 == 0) {
                    _leds->leftLedsOn();
                } else {
                    _leds->rightLedsOn();
                }
            }
        }
    }
    while(touched);
    
    return touchedCount;
}

void TouchSensors::processTouch(int touchedCount) {
    if (touchedCount > _veryLongTouchThreshold) {
        doVeryLongTouchAction();
    } else if (touchedCount > _longTouchThreshold) {
        doLongTouchAction();
    } else {
        doTouchedAction();
    }
}

void TouchSensors::doTouchedAction() {
    Particle.publish("Status", "Touched");
}

void TouchSensors::doLongTouchAction() {
    Particle.publish("Status", "Long touch");
}

void TouchSensors::doVeryLongTouchAction() {
    Particle.publish("Status", "VERY Long touch");
}

void TouchSensors::touchInterrupt() {
    _touched = true;
}


///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
TouchSensorsV1::TouchSensorsV1(LedHandler* leds) : TouchSensors(D5, D6, leds) {

}

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////s
TouchSensorsV2::TouchSensorsV2(LedHandler* leds) : TouchSensors(D5, D6, leds) {

};
