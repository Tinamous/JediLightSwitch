// Responsible for handling temperature and humidity measurements.

#ifndef EnvironmentSensors_H
#define EnvironmentSensors_H

#include "HTU21D/HTU21D.h"
#include "LedHandler.h"
#include "SenMLBuilder.h"


///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
class EnvironmentSensors {
public:
    EnvironmentSensors(LedHandler* leds);
    virtual bool init();
    virtual void readTemperatureAndHumidity(bool publish);
    virtual void readAnalogLightLevel(bool publish);
    
    // Append sensor measurements to SenML Builder
    // for publication.
    virtual void appendSenML(SenMLBuilder* builder);
    
    uint16_t getTemperature();
    uint16_t getHumidity();
    int getAnalogLightLevel();
    
protected:
    int _analogLightLevel = 0;

private:
    HTU21D _humiditySensor;
    LedHandler* _leds;
    uint16_t _humidity = 0;
    uint16_t _temperature = 0;
};

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
class EnvironmentSensorsV1 : public EnvironmentSensors {
public:
    EnvironmentSensorsV1(LedHandler* leds);
};

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
class EnvironmentSensorsV2 : public EnvironmentSensors {
public:
    EnvironmentSensorsV2(LedHandler* leds);
    bool init() override;
    void readAnalogLightLevel(bool publish) override;
    void appendSenML(SenMLBuilder* builder) override;
private:
    int _analogLightLevelPin = A1;
};

#endif