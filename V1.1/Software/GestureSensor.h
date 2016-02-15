// Responsible for handling gasture and gesture sensor light levels.

#ifndef GestureSensor_H
#define GestureSensor_H

#include "LedHandler.h"
#include "SparkFun_APDS9960.h"
#include "SenMLBuilder.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
class GestureSensor {
public:
    GestureSensor(int gestureInterruptPin, LedHandler* leds);
    ~GestureSensor();
    virtual bool init();
    void checkForGesture();
    void readLightLevel(bool publish);
    int getAmbientLightLevel();
    bool isDark();
    bool isLight();
    
    // Append sensor measurements to SenML Builder
    // for publication.
    virtual void appendSenML(SenMLBuilder* builder);
private:
    bool setupLightAndGesture();
    void handleGesture();
    
    void publishLightLevel();
    void readLightColors(bool publish);
    void publishColorLevels();
    void readAmbientLight();
    void readRedLight();
    void readGreenLight();
    void readBlueLight();
    void readProximity();
    
    void processGesture();
    
    // Light level thresholds
    int _ambientLightLowThreshold = 10;
    int _ambientLightHighThreshold = 15;

    // APDS-9960 variables for light/proximity and gesture sensing.
    SparkFun_APDS9960 _apds = SparkFun_APDS9960();
    
    LedHandler* _leds;
    
    int _errorCode = 0;
    // Set to -1 to indicate the level has not been measured.
    int _ambientLight = -1;
    int _redLevel = -1;
    int _greenLevel = -1;
    int _blueLevel = -1;
    int _proximity = -1;
    
    // Interupt
    void gestureInterrupt();
    int _gestureInterruptPin;
    volatile bool _gestureIsrFlag = false;
};

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
class GestureSensorV1 : public GestureSensor {
public:
    GestureSensorV1(LedHandler* leds);
};

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
class GestureSensorV2 : public GestureSensor {
public:
    GestureSensorV2(LedHandler* leds);
};

#endif