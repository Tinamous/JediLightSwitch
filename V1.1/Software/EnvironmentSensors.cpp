// Responsible for handling temperature and humidity measurements.

 #include "application.h"
 #include "EnvironmentSensors.h"
 
///////////////////////////////////////////////////////////////////////////////////
// Base class
///////////////////////////////////////////////////////////////////////////////////

// Ctor.
EnvironmentSensors::EnvironmentSensors(LedHandler* leds) {
     _leds = leds;
}

bool EnvironmentSensors::init() {
    // Setup I2C as master
    if ( !Wire.isEnabled() ) {
        Wire.setSpeed(CLOCK_SPEED_400KHZ);
        Wire.begin();
    }
    
        // Initialize Temperature and humidity
    if (!_humiditySensor.begin()) {
        Particle.publish("Setup", "Temperature/Humidity sensor setup failed.");
        delay(250);
        RGB.color(255, 0, 00);
        return false;
    }
    
    return true;
}

void EnvironmentSensors::readTemperatureAndHumidity(bool publish) {
    float h = _humiditySensor.readHumidity();
    if (h < 100) {
        _humidity = h;
    } else {
        // Invalid / error.
        RGB.color(255, 0, 0);
    }
    
    float t = _humiditySensor.readTemperature();
    if (t < 100) {
        _temperature = t;
    } else {
        // Invalid / error.
        RGB.color(255, 0, 0);
    }
    
    if (publish) {
        Particle.publish("senml", "{e:[{'n':'Temp','v':'" + String(_temperature) + "'},{'n':'RH','v':'" + String(_humidity) + "'}]}");
    }
}

void EnvironmentSensors::readAnalogLightLevel(bool publish) {
    // No action for V1 hardware.
}

void EnvironmentSensors::appendSenML(SenMLBuilder* builder) {
   // "{e:[{'n':'Temperature','v':'" + String(_temperature) + "'},{'n':'Humidity','v':'" + String(_humidity) + "'}]}"
   builder->add("T", _temperature);
   builder->add("RH", _humidity);
}

uint16_t EnvironmentSensors::getTemperature() {
    return _temperature;    
}

uint16_t EnvironmentSensors::getHumidity() {
    return _humidity;
}

int EnvironmentSensors::getAnalogLightLevel() {
    return _analogLightLevel;
}

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
// ctor
EnvironmentSensorsV1::EnvironmentSensorsV1(LedHandler* leds) : EnvironmentSensors(leds) {
}


///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
// ctor
EnvironmentSensorsV2::EnvironmentSensorsV2(LedHandler* leds) : EnvironmentSensors(leds) {
}

bool EnvironmentSensorsV2::init() {
    // initialise the base class
    EnvironmentSensors::init();
    pinMode(_analogLightLevelPin, INPUT);
    return true;
}

// Override to read actual analog light level as it's supported on 
// V2 hardware.
void EnvironmentSensorsV2::readAnalogLightLevel(bool publish) {
    _analogLightLevel = analogRead(_analogLightLevelPin);
}

void EnvironmentSensorsV2::appendSenML(SenMLBuilder* builder) {
    EnvironmentSensors::appendSenML(builder);
    builder->add("LightAnalog", _analogLightLevel);
}