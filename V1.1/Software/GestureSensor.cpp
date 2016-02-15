 #include "application.h"
 #include "GestureSensor.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
// Ctor.
GestureSensor::GestureSensor(int gestureInterruptPin, LedHandler* leds) {
    _gestureInterruptPin = gestureInterruptPin;
    _leds = leds;
}

/**
 * @brief Destructor
 */
GestureSensor::~GestureSensor() {
}

bool GestureSensor::init() {
    
    // Already has 10k pull-up.
    pinMode(_gestureInterruptPin, INPUT); 
    
    if ( !Wire.isEnabled() ) {
        Wire.setSpeed(CLOCK_SPEED_400KHZ);
        Wire.begin();
    }

    if (!setupLightAndGesture()) {
        Particle.publish("Setup", "Setup Light and Gesture failed. APDS-9960 Error code: " + String(_errorCode));
        return false;
    }
    
     // Now everything is set-up attach the intterupts
    auto interruptHandler = std::bind(&GestureSensor::gestureInterrupt, this);
    attachInterrupt(_gestureInterruptPin, interruptHandler, FALLING);
    
    return true;
}

// Initialise Gesture/Light/Proximity Sensor
bool GestureSensor::setupLightAndGesture() {
    
    // Initialize APDS-9960 (configure I2C and initial values)
    int errorCode = _apds.init();
    if ( _errorCode < 0) {
        Particle.publish("Setup", "Error during APDS-9960 init. APDS-9960 Error code: " + String(errorCode));
        _errorCode = 1;
        RGB.color(255, 0, 00);
        return false;
    }
    
    // Enable light sensing
    if ( !_apds.enableLightSensor(false) ) {
        RGB.color(255, 0, 00);
        _errorCode = 3;
        Particle.publish("Setup", "Error enabling light sensor");
        return false;
    }
    
    // Interrup when light below the low  threshold to say it's dark
    //_apds.setLightIntLowThreshold(_ambientLightLowThreshold);
    // Interrup when light above the high threshold to say it's now light
    //_apds.setLightIntHighThreshold(_ambientLightHighThreshold);
    
    ///
    // Set-up proximity
    // ---
    // Adjust the Proximity sensor gain
    if ( !_apds.setProximityGain(PGAIN_4X) ) { 
        RGB.color(255, 0, 00);
        _errorCode = 4;
        Particle.publish("Setup", "Error  trying to set PGAIN");
        return false;
    } 

    // Start running the APDS-9960 gesture sensor engine
    if (!_apds.enableGestureSensor(true) ) {
        RGB.color(255, 0, 00);
        _errorCode = 8;
        Particle.publish("Setup", "Enable Gesture setup failed.");
        return false;
    }
    
    _apds.clearAmbientLightInt();
    _apds.clearProximityInt();
    
    
    Particle.publish("Setup", "Gesture set-up complete");
    
    return true;
}

void GestureSensor::checkForGesture() {
        // If gesture/proximity/light interrupt flag.
    if (_gestureIsrFlag) {
        //_leds->debugLedOn();
        handleGesture();
    } else if (_apds.isGestureAvailable()) {
        //_leds->debugLedOn();
    }
    
        _apds.clearAmbientLightInt();
    _apds.clearProximityInt();
    
    //processGesture();
    //delay(150);
    //_leds->debugLedOff();
}

void GestureSensor::handleGesture() {
    
    detachInterrupt(_gestureInterruptPin);
    
    // Check the light level as that will cause an interrupt.
    // leave it up to the main loop to determine if light/dard.
    readLightLevel(false);

    // now check for a gesture...
    //readProximity();
    //checkProximity();
    processGesture();
    
    _apds.clearAmbientLightInt();
    _apds.clearProximityInt();
    
    // Pause to allow user to see LEDs
    delay(500);
    _gestureIsrFlag = 0;
    _leds->ledsOff();
    
    auto interruptHandler = std::bind(&GestureSensor::gestureInterrupt, this);
    attachInterrupt(_gestureInterruptPin, interruptHandler, FALLING);
}

void GestureSensor::readLightLevel(bool publish) {
    
    // Read light levels
    readAmbientLight();
    
    // Publish details
    if (publish) {
        publishLightLevel();
    }
}

void GestureSensor::publishLightLevel() {
    Particle.publish("senml", "{e:[{'n':'LightLevel','v':'" + String(_ambientLight) + "'}]}");
}

void GestureSensor::readLightColors(bool publish) {
    
    readRedLight();
    
    readGreenLight();
    
    readBlueLight();

    // Publish details
    if (publish) {
        publishColorLevels();
    }
}

void GestureSensor::publishColorLevels() {
    Particle.publish("senml", "{e:[{'n':'LightLevel','v':'" + String(_ambientLight) + "'},{'n':'Red','v':'" + String(_redLevel) + "'},{'n':'Green','v':'" + String(_greenLevel) + "'},{'n':'Blue','v':'" + String(_blueLevel) + "'},{'n':'Proximity','v':'" + String(_proximity) + "'}]}");    
}

void GestureSensor::readAmbientLight() {
    
    uint16_t value = 0;
    if (_apds.readAmbientLight(value)) {
        _ambientLight = value;
    } else {
        _errorCode = 100;
        RGB.color(255, 0, 0);
        // Particle.publish("Status", "Error reading ambient light");
    }
}

void GestureSensor::readRedLight() {
    uint16_t value = 0;
    if (_apds.readRedLight(value)) {
        _redLevel = value;
    } else {
        _errorCode = 101;
        RGB.color(255, 0, 0);
    }
}

void GestureSensor::readGreenLight() {
    uint16_t value = 0;
    if (_apds.readGreenLight(value)) {
        _greenLevel = value;
    } else {
        _errorCode = 102;
        RGB.color(255, 0, 0);
    }
}

void GestureSensor::readBlueLight() {
    uint16_t value = 0;
    if (_apds.readBlueLight(value)) {
        _blueLevel = value;
    } else {
        _errorCode = 103;
        RGB.color(255, 0, 0);
    }
}

void GestureSensor::readProximity() {
    if ( !_apds.clearProximityInt() ) {
        RGB.color(255, 0, 0);
    }
    
    // Check proximity.
    uint8_t value = 0;
    if ( _apds.readProximity(value) ) {
        _proximity = value;
    } 
}

void GestureSensor::processGesture() {
    if (_apds.isGestureAvailable()) {
        _leds->ledsOff();
        
        int gestureValue = _apds.readGesture();
        //Particle.publish("Status", String(gestureValue));

        switch (gestureValue) {
            case DIR_NONE: // 0
                //Particle.publish("Status", "Swipe None");
                // Skip the delay.
                return;
            case DIR_LEFT: // 1
                _leds->leftLedsOn();
                Particle.publish("Status", "Swipe Left");
                break;
            case DIR_RIGHT: // 2
                _leds->rightLedsOn();
                Particle.publish("Status", "Swipe Right");
                break;
            case DIR_UP: // 3
                _leds->topLedsOn();
                Particle.publish("Status", "Swipe Up");
                break;
            case DIR_DOWN: // 4
                _leds->bottomLedsOn();
                Particle.publish("Status", "Swipe Down");
                break;
            case DIR_NEAR: // 5
                _leds->allLedsOn();
                Particle.publish("Status", "Swipe Near");
                break;
            case DIR_FAR: // 6
                _leds->allLedsOn();
                Particle.publish("Status", "Swipe Far");
                break;
            case DIR_ALL: // 7
                Particle.publish("Status", "Swipe ALL!");
                break;
         // default:
            //Serial.println("NONE");
        }
    } 
}

// Interrupt from gesture sensor.
void GestureSensor::gestureInterrupt() {
    _gestureIsrFlag = true;
}

int GestureSensor::getAmbientLightLevel() {
    return _ambientLight;
}

bool GestureSensor::isDark() {
    // Unknown. Not measured
    if (_ambientLight < 0) {
        return false;
    }
    
    return _ambientLight < _ambientLightLowThreshold;
}

bool GestureSensor::isLight() {
    // Unknown. Not measured
    if (_ambientLight < 0) {
        return false;
    }
    
    return _ambientLight > _ambientLightHighThreshold;
}

void  GestureSensor::appendSenML(SenMLBuilder* builder) {
    // "{e:[{'n':'LightLevel','v':'" + String(_ambientLight) + "'}]}"
    builder->add("Light", _ambientLight);
}

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
// Constructor for V1 handler which sets up the base class
// with the known IO pins or the V1.1 board.
GestureSensorV1::GestureSensorV1(LedHandler* leds) : GestureSensor(D2, leds) {
}

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
// Constructor for V2  handler which sets up the base class
// with the known IO pins or the V1.2 board.
GestureSensorV2::GestureSensorV2(LedHandler* leds) : GestureSensor(D2, leds) {
}