// This #include statement was automatically added by the Spark IDE.
#include "HTU21D/HTU21D.h"

// This #include statement was automatically added by the Particle IDE.
#include "SparkFun_APDS9960.h"

// D0/D1 used for I2C
int gestureInteruptPin = D2;
int topRightLed = D3;
int topLeftLed = D4;
int touchInput = D5;

// Spark variables
int errorCode = 0;
int ambientLight = 0;
int redLevel = 0;
int greenLevel = 0;
int blueLevel = 0;
int proximity = 0;

int clockSpeed = CLOCK_SPEED_400KHZ;

// Interrup from gesture sensor flag.
volatile int gestureIsrFlag = 0;
volatile int touchIsrFlag = 0;

#define PROX_INT_HIGH   50 // Proximity level for interrupt
#define PROX_INT_LOW    0  // No far interrupt

// APDS-9960 variables for light/proximity and gesture sensing.
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
uint8_t proximity_data = 0;
bool useLightAndGesture = false;

// Temperature and humidity
HTU21D humiditySensor;
// Spark variables for tracking humidity and temperature
uint16_t humidity = 0;
uint16_t temperature = 0;

// Loop counter 
int loopCounter = 0;

void setup() {
    //Serial.begin(9600);
    //Serial.println();
    //Serial.println("--------------------------------");
    //Serial.println("APDS-9960 - GestureTest");
    //Serial.println("--------------------------------");
    //Serial.println();
  
    // Define Particle variables for Tinamous to access.
    /*
    Particle.variable("errorCode", &errorCode, INT);
    Particle.variable("ambientLight", &ambientLight, INT);
    Particle.variable("redLevel", &redLevel, INT);
    Particle.variable("greenLevel", &greenLevel, INT);
    Particle.variable("blueLevel", &blueLevel, INT);
    Particle.variable("proximity", &proximity, INT);
    Particle.variable("humidity", &humidity, INT);
    Particle.variable("temperature", &temperature, INT);
    */
    
    // Take control of the spark LED and make it dimmer as it's very
    // bright by default (Core only, Photon not so bright)
    RGB.control(true);
    RGB.brightness(100);
    
    // Set red to show initializing.
    RGB.color(255, 0, 0);
  
    // Set output mode for D7 LED.
    pinMode(D7, OUTPUT);
    pinMode(topLeftLed, OUTPUT);
    pinMode(topRightLed, OUTPUT);
    pinMode(touchInput, INPUT);
    digitalWrite(topLeftLed, HIGH);
    digitalWrite(topRightLed, HIGH);
    
    // Setup I2C as master.
    Wire.setSpeed(clockSpeed);
    Wire.begin();
    
    useLightAndGesture = setupLightAndGesture();
    if (!useLightAndGesture) {
        Particle.publish("Setup", "APDS-9960 setup error. Disabled.");
        RGB.color(255, 0, 00);
    }
    // Allow the status message to be sent.
    delay(1000);
    
    // Now everything is set-up attach the intterupts
    //attachInterrupt(touchInteruptPin, touchInterup, FALLING);
    pinMode(gestureInteruptPin, INPUT); // Already has 10k pull-up.
    attachInterrupt(gestureInteruptPin, gestureInterupt, FALLING);
    
    // Initialize Temperature and humidity
    if (!humiditySensor.begin()) {
        RGB.color(255, 0, 00);
    }
    
    // TODO: Attach interrupt to touchInput...
    attachInterrupt(touchInput, touchInterupt, RISING);
    
    //Serial.println("Setup Complete.");
    delay(500);
    digitalWrite(topLeftLed, LOW);
    digitalWrite(topRightLed, LOW);
}

void loop() {
    
    // Flash the onboard tricolor LED blue to indicate running.
    RGB.color(0, 0, 255);
    delay(50);
    RGB.color(0, 0, 0);
    delay(50);

    // Read light, temperature and humidity levels every x times around the loop. 
    if (loopCounter > 300) {
        // Flash the LEDs when measuring to give feedback
        digitalWrite(topLeftLed, HIGH);
        digitalWrite(topRightLed, HIGH);
    
        // Read the light level every loop but 
        // only publish the update every 50 times around.
        readLightLevel(true);
        readTemperatureAndHumidity(true);
        loopCounter = 0;
        delay(250);
        
        ledsOff();
     }
     
    // If gesture interrupt flag.
    if (gestureIsrFlag) {
        //detachInterrupt(gestureInteruptPin);
        noInterrupts();
        
        //readProximity();
        handleGesture();
        
        delay(250);
        ledsOff();
        gestureIsrFlag = 0;
        
        interrupts();
        //attachInterrupt(gestureInteruptPin, gestureInterupt, FALLING);
    }
    
    if (touchIsrFlag) {
        noInterrupts();
        
        digitalWrite(topLeftLed, HIGH);
        digitalWrite(topRightLed, HIGH);
        
        Particle.publish("Status", "Touched");
        delay(250);
        
        ledsOff();
        touchIsrFlag = 0;
        interrupts();
    }
    
    loopCounter++;
} 

void ledsOff() {
    digitalWrite(topLeftLed, LOW);
    digitalWrite(topRightLed, LOW);
    digitalWrite(D7, LOW);
}

// ***********************************************************************
// Temperature and Humidity
// ***********************************************************************

void readTemperatureAndHumidity(bool publish) {
    float h = humiditySensor.readHumidity();
    if (h < 100) {
        humidity = h;
    } else {
        RGB.color(255, 0, 0);
    }
    
    float t = humiditySensor.readTemperature();
    if (t<100) {
        temperature = t;
    } else {
        RGB.color(255, 0, 0);
    }
    
    if (publish) {
        Particle.publish("senml", "{e:[{'n':'Temperature','v':'" + String(temperature) + "'},{'n':'Humidity','v':'" + String(humidity) + "'}]}");
    }
    
    //humidity = humiditySensor.readHumidity() * 100;
    //temperature = humiditySensor.readTemperature() * 100;
}

// ***********************************************************************
// Light and Gesture
// ***********************************************************************

// Initialise Gesture/Light/Proximity Sensor
bool setupLightAndGesture() {
    
    RGB.color(0, 0, 0);
    Particle.publish("Setup", "APDS-9960 Setup.");
    
    // Initialize APDS-9960 (configure I2C and initial values)
    errorCode = apds.init();
    if ( errorCode < 0) {
        Particle.publish("Setup", "Error during APDS-9960 init. APDS-9960 Error code: " + String(errorCode));
        errorCode = 1;
        RGB.color(255, 0, 00);
        return false;
    }
    
    // Start running the APDS-9960 gesture sensor engine
    if (!apds.enableGestureSensor(true) ) {
        RGB.color(255, 0, 00);
        errorCode = 8;
        Particle.publish("Setup", "Enable Gesture setup failed.");
        return false;
    }
    
    // Enable light sensing
    if ( !apds.enableLightSensor(false) ) {
        RGB.color(255, 0, 00);
        errorCode = 3;
        Particle.publish("Setup", "Error enabling light sensor");
        return false;
    }
    
    // Adjust the Proximity sensor gain
    //if ( !apds.setProximityGain(PGAIN_2X) ) { // PGAIN_2X
    //    RGB.color(255, 0, 00);
    //    errorCode = 4;
    //    Particle.publish("Setup", "Error  trying to set PGAIN");
    //    return false;
    //} 
      
    // Set proximity interrupt thresholds
    //if ( !apds.setProximityIntLowThreshold(PROX_INT_LOW) ) {
    //    RGB.color(255, 0, 00);
    //    errorCode = 5;
    //    Particle.publish("Setup", "Error  trying to set low threshold");
    //    return false;
    //}
    
    //if ( !apds.setProximityIntHighThreshold(PROX_INT_HIGH) ) {
    //    RGB.color(255, 0, 00);
    //    errorCode = 6;
     //   Particle.publish("Setup", "Error  trying to set high threshold");
     //   return false;
    //}
      
      /*
    // Start running the APDS-9960 proximity sensor (interrupts)
    if ( !apds.enableProximitySensor(true) ) {
        RGB.color(255, 0, 00);
        errorCode = 7;
        Spark.publish("Setup", "Enable Proximity setup failed.");
        return false;
    }*/
    
    // Configure for Left/Right swipe only.


    // Wait for initialization and calibration to finish
    Particle.publish("Setup", "APDS-9960 Setup completed successfully.");
    
    return true;
}

void readLightLevel(bool publish) {
    
    // TODO: Determine if proximity is triggered and skip measurement if so
    // so that measurement isn't taken with blocked sensor.
    
    // Read light levels
    readAmbientLight();
    readRedLight();
    readGreenLight();
    readBlueLight();

    // Publish details
    if (publish) {
        Particle.publish("senml", "{e:[{'n':'LightLevel','v':'" + String(ambientLight) + "'},{'n':'Red','v':'" + String(redLevel) + "'},{'n':'Green','v':'" + String(greenLevel) + "'},{'n':'Blue','v':'" + String(blueLevel) + "'}]}");
    }
}

void readAmbientLight() {
    if (apds.readAmbientLight(ambient_light)) {
        ambientLight = ambient_light;
    } else {
        errorCode = 100;
        RGB.color(255, 0, 0);
        Particle.publish("Error", "Error reading ambient light level");
        delay(250); 
    }
}

void readRedLight() {
    if (apds.readRedLight(red_light)) {
        redLevel = red_light;
    } else {
        errorCode = 101;
        RGB.color(255, 0, 0);
        Particle.publish("Error", "Error reading red light level");
        delay(250); 
    }
}

void readGreenLight() {
    if (apds.readGreenLight(green_light)) {
        greenLevel = green_light;
    } else {
        errorCode = 102;
        RGB.color(255, 0, 0);
        Particle.publish("Error", "Error reading green light level");
        delay(250); 
    }
}

void readBlueLight() {
    if (apds.readBlueLight(blue_light)) {
        blueLevel = blue_light;
    } else {
        errorCode = 103;
        RGB.color(255, 0, 0);
        Particle.publish("Error", "Error reading blue light level");
        delay(250); 
    }
}

void readProximity() {
    gestureIsrFlag = 0;
    if ( !apds.clearProximityInt() ) {
        Particle.publish("Error", "Error clearing interrupt");
        RGB.color(255, 0, 0);
    }
    
    // Check proximity.
    if ( apds.readProximity(proximity_data) ) {
        // Set LED blue level to proximity level
        RGB.color(0, 0, proximity_data);
        proximity = proximity_data;
        
        Spark.publish("Proximity", String(proximity_data) ); 
        delay(100);
        //Serial.print("Proximity:");
        //Serial.println(proximity_data);
    } 
}

void handleGesture() {
    if (apds.isGestureAvailable()) {
        //Serial.println("G");
        //RGB.color(255, 255, 0);
        digitalWrite(topLeftLed, LOW);
        digitalWrite(topRightLed, LOW);
        
        int gestureValue = apds.readGesture();
        //Particle.publish("Status", String(gestureValue));

        switch (gestureValue) {
            case DIR_NONE: // 0
                //Particle.publish("Status", "Swipe None");
                // Skip the delay.
                return;
            case DIR_LEFT: // 1
                RGB.color(0, 255, 0);
                digitalWrite(topLeftLed, HIGH);
                digitalWrite(topRightLed, LOW);
                Particle.publish("Status", "Swipe Left");
                break;
            case DIR_RIGHT: // 2
                RGB.color(255, 0, 0);
                digitalWrite(topLeftLed, LOW);
                digitalWrite(topRightLed, HIGH);
                Particle.publish("Status", "Swipe Right");
                break;
            case DIR_UP: // 3
                Particle.publish("Status", "Up");
                break;
            case DIR_DOWN: // 4
                Particle.publish("Status", "Down");
                break;
            case DIR_NEAR: // 5
                Particle.publish("Status", "Near");
                break;
            case DIR_FAR: // 6
                Particle.publish("Status", "Far");
                break;
            case DIR_ALL: // 7
                Particle.publish("Status", "ALL!");
                break;
         // default:
            //Serial.println("NONE");
        }
        
        // Send publish info delay
        delay(100);
    } 
}

// Gesture Interrupt
void gestureInterupt() {
    // Ignore gesture for now to try and trace stability issues.
    // gestureIsrFlag = 1;
}

void touchInterupt() {
    touchIsrFlag = 1;
}