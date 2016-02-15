// This #include statement was automatically added by the Particle IDE.
#include "SenMLBuilder.h"

// This #include statement was automatically added by the Particle IDE.
#include "IrBlaster.h"

// This #include statement was automatically added by the Particle IDE.
#include "LightSensor.h"

// This #include statement was automatically added by the Particle IDE.
#include "LedHandler.h"

// This #include statement was automatically added by the Particle IDE.
#include "PowerManagement.h"

// This #include statement was automatically added by the Particle IDE.
#include "GestureSensor.h"

// This #include statement was automatically added by the Particle IDE.
#include "TouchSensors.h"

// This #include statement was automatically added by the Particle IDE.
#include "EnvironmentSensors.h"

// This isn't neaded but Partile build complains if it's not.
// It's already included in GestureSensor.h
#include "SparkFun_APDS9960.h"

///////////////////////////////////////////////////////////////////////////////////
// 0: V1.0 board (with logo on)
// 1: V1.1 board (2 vertical front sensors, LED in each corner)
// 2: V1.2 board (5 horizontal front sensors)
const int HARDWARE_REV = 2;
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Hardware abstraction.
LedHandler* leds;
EnvironmentSensors* environmentSensors;
TouchSensors* touchSensors;
GestureSensor* gestureSensor;
PowerManagement* powerManagement;
IrBlaster* irBlaster;
///////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Particle functions
// Flash the user facing LEDs
int flash(String command);
// Switch on the internal LED to make the box glow.
int glow(String command);
// Turn off User facing LEDs/Internal LED
int off(String command);
///////////////////////////////////////////////////////////////////////////////////

// Build a senml document for publishing
// NB: Max document size for publishing is 255 bytes.
SenMLBuilder senMLBuilder = SenMLBuilder();

// If the lightswitch things it is currently dark.
bool isDark = false;

// Loop counter 
int loopCounter = 0;
int publishMeasurementsCounter = 0;

// If measurements should be taken on the next run through loop.
bool timeToTakeMeasurements = false;

// Measurement timer. Every n seconds.
Timer takeMeasurementsTimer(20000, takeMeasurementsTick);

// ***********************************************************************
// Setup
// ***********************************************************************
void setup() {
    
    setupHAL();

    setupLeds();
    
    setupGestureSensor();
    
    setupTouch();
    
    setupEnvironmentSensors();
    
    setupIrBlaster();
    
    setupParticleFunctions();
    
    //attachInterrupt(D2, interruptHandler, FALLING);
    
    setupComplete();
    
    // Force and an initial measurement reading.
    //timeToTakeMeasurements = true;
}

void interruptHandler() {
    leds->debugLedOn();
}

// Setup the hardware abstraction layer classes.
void setupHAL() {
    if (HARDWARE_REV == 2) {
        leds = new LedHandlerV2();
        gestureSensor = new GestureSensorV2(leds);
        environmentSensors = new EnvironmentSensorsV2(leds);
        irBlaster = new IrBlasterV2();
        touchSensors = new TouchSensorsV2(leds);
        powerManagement = new PowerManagementV2();
    } else {
        leds = new LedHandlerV1();
        gestureSensor = new GestureSensorV1(leds);
        environmentSensors = new EnvironmentSensorsV1(leds);
        irBlaster = new IrBlasterV1();
        touchSensors = new TouchSensorsV1(leds);
        powerManagement = new PowerManagementV1();
    }
    
}

void setupLeds() {
    if (!leds->init()) {
        Particle.publish("Setup", "Leds setup error.");
        RGB.color(255, 0, 00);
    }
}

void setupGestureSensor() {
    if (!gestureSensor->init()) {
        Particle.publish("Setup", "APDS-9960 setup error.");
        RGB.color(255, 0, 00);
    } 
}

void setupEnvironmentSensors() {
    if (!environmentSensors->init()) {
        Particle.publish("Setup", "EnvironmentSensors setup failed.");
        RGB.color(255, 0, 00);
    }
}

void setupIrBlaster() {
    if (!irBlaster->init()) {
        Particle.publish("Setup", "IR Blaster setup failed.");
        RGB.color(255, 0, 00);
    }
}

void setupTouch() {
    if (!touchSensors->init()) {
        Particle.publish("Setup", "Touch sensor setup faield.");
        RGB.color(255, 0, 00);
    }
}

void setupPowerManagement() {
    if (!powerManagement->init()) {
        Particle.publish("Setup", "Power management setup failed.");
        RGB.color(255, 0, 00);
    }
}

// Set-up internet callable Particle functions
void setupParticleFunctions() {
    Particle.function("flash", flash);
    Particle.function("glow", glow);
    Particle.function("off", off);
}

void setupComplete() {
    // Complete setup.
    Particle.publish("Setup", "Jedi Light Switch Set-up complete. V1.2.15");
    
    // Short delay to allow the LEDs to be seen and
    // publishing to happen.
    delay(500);
    leds->ledsOff();
    
    // Start the measurements timer
    takeMeasurementsTimer.start();
}

// ***********************************************************************
// Main Loop
// ***********************************************************************
void loop() {
    
    gestureSensor->checkForGesture();
    touchSensors->checkIfTouched();
    
    if (timeToTakeMeasurements) {
        takeMeasurements();
    }
    
    // Check to see if the light level has changes
    // Uses Gesture sensor and/or environment sensors which are updated 
    // in the background.
    checkLightDarkLevel();

    loopCounter++;
    delay(250);
    leds->debugLedOff();
}

// Called every n seconds by the timer.
// to take light/temperature/humidity measurements and 
// publish them.
// Does not actually take the measurements as it publishes
// and this can cause delays which are undesirable
void takeMeasurementsTick() {
    timeToTakeMeasurements = true;
}

// Called every n seconds by the timer.
// to take light/temperature/humidity measurements and 
// publish them.
void takeMeasurements() {
    
    timeToTakeMeasurements = false;
    
    // Flash the LEDs when measuring to give feedback
    //allLedsOn(); // temp debug to show reading
    //leds->debugLedOn();
    
    // Read the light level every loop but 
    // only publish the update every n times around.
    gestureSensor->readLightLevel(false);
    
    //gestureSensor->readLightColors(publishMeasurements);
    environmentSensors->readTemperatureAndHumidity(false);
    
    environmentSensors->readAnalogLightLevel(false);
    
    powerManagement->readPowerState(false);
    
    // publish the measurements as one block if appropriate and reset the counters
    if (publishMeasurementsCounter > 0) {
        // Start a nice fresh senml document.
        senMLBuilder.begin();
        environmentSensors->appendSenML(&senMLBuilder);
        gestureSensor->appendSenML(&senMLBuilder);
        powerManagement->appendSenML(&senMLBuilder);
        senMLBuilder.publish();
        
        publishMeasurementsCounter = 0;
    }
    publishMeasurementsCounter++;
    
    // Delay to allow the leds to be seen to flash
    delay(250);
    //leds->debugLedOff();
}

// ***********************************************************************
// Check to see if it is light/dark.
// ***********************************************************************
void checkLightDarkLevel() {
    // TODO: Make ambientLightThreshold configurable via particle function.
    
    if (gestureSensor->isDark()) {
        if (!isDark) {
            // Below "Dark" level
            // Enable glowing.
            leds->glow();
        
            Particle.publish("Status", "It is now dark.");
            isDark = true;
        }
    }  else if (gestureSensor->isLight()) {
        if (isDark) {
            // Above "Light" level
            // Disable glowing.
            // TODO: But not if requested via the glow particle.function 
            leds->noglow();
        
            Particle.publish("Status", "It is now light.");
            isDark = false;
        }
        
        return;
    }
}

///////////////////////////////////////////
// Particle Function handles
///////////////////////////////////////////

// Flash the user facing LEDs
int flash(String command) {
    
    for (int i = 0; i<10; i++) {
        leds->ledsOff();
        leds->topLedsOn();
        delay(250);
        
        leds->ledsOff();
        leds->bottomLedsOn();
        delay(250);
    }
    
    leds->ledsOff();
    
    Particle.publish("Status", "Flashed LEDs");
    return 1;
}

// Switch on the internal LED to make the box glow.
int glow(String command) {
    if (command == "on") {
        leds->glow();
        Particle.publish("Status", "Glowing");
        return 1;
    } else if (command == "rainbow") {
        leds->rainbowGlow();
        Particle.publish("Status", "Rainbow Glowing");
        return 2;
    }
    else {
        leds->noglow();
        Particle.publish("Status", "Not Glowing");
        return 0;
    }
    
    Particle.publish("Status", "No command for glow :-(");
    return -1;
}

// Turn off User facing LEDs/Internal LED
int off(String command) {
    
    leds->ledsOff();
    leds->noglow();
    Particle.publish("Status", "LEDs Off");
    return 0;
}