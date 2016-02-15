// Responsible for handling the interface with the 4 UI LEDs in the corners of the board.

#include "application.h"
#include "LedHandler.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
// Ctor.
LedHandler::LedHandler(int topLeftLed, int topRightLed, int bottomLeftLed, int bottomRightLed, int debugLed) {
     _topLeftLed = topLeftLed;
     _topRightLed =  topRightLed;
     _bottomLeftLed = bottomLeftLed;
     _bottomRightLed = bottomRightLed;
     _debugLed = debugLed;  
}


/**
 * @brief Destructor
 */
LedHandler::~LedHandler() {
}

/**
 * @brief Configures I2C communications and initializes registers to defaults
 *
 * @return True if initialized successfully. False otherwise.
 */
bool LedHandler::init() {
        // Take control of the Photons LED.
    RGB.control(true);
    RGB.brightness(100);
    RGB.color(0, 0, 0);
    
        // Set output mode for D7 LED.
    pinMode(_debugLed, OUTPUT);
    
    // User facing LEDs
    pinMode(_topLeftLed, OUTPUT);
    pinMode(_topRightLed, OUTPUT);
    pinMode(_bottomLeftLed, OUTPUT);
    pinMode(_bottomRightLed, OUTPUT);
    
    // Set default logic levels for the IO pins.
    // Switch all the user facing LEDs on during setup
    digitalWrite(_topLeftLed, HIGH);
    digitalWrite(_topRightLed, HIGH);
    digitalWrite(_bottomLeftLed, HIGH);
    digitalWrite(_bottomRightLed, HIGH);
    
    return true;
}

void LedHandler::debugLedOn() {
    // NB: This clashes with bottom right
    digitalWrite(_debugLed, HIGH);
}

void LedHandler::debugLedOff() {
    // NB: This clashes with bottom right
    digitalWrite(_debugLed, LOW);
}

void LedHandler::ledOn(int led) {
    switch (led) {
        case 1:
            digitalWrite(_topLeftLed, HIGH);
            break;
        case 2:
            digitalWrite(_topRightLed, HIGH);
            break;
        case 3:
            digitalWrite(_bottomRightLed, HIGH);
            break;
        case 4:
            digitalWrite(_bottomLeftLed, HIGH);
            break;
    }
}

void LedHandler::ledOff(int led) {
    switch (led) {
        case 1:
            digitalWrite(_topLeftLed, LOW);
            break;
        case 2:
            digitalWrite(_topRightLed, LOW);
            break;
        case 3:
            digitalWrite(_bottomRightLed, LOW);
            break;
        case 4:
            digitalWrite(_bottomLeftLed, LOW);
            break;
    }
}

void LedHandler::allLedsOn() {
    digitalWrite(_topLeftLed, HIGH);
    digitalWrite(_topRightLed, HIGH);
    digitalWrite(_bottomLeftLed, HIGH);
    digitalWrite(_bottomRightLed, HIGH);
}

void LedHandler::topLedsOn() {
    digitalWrite(_topLeftLed, HIGH);
    digitalWrite(_topRightLed, HIGH);
}

void LedHandler::bottomLedsOn() {
    digitalWrite(_bottomLeftLed, HIGH);
    digitalWrite(_bottomRightLed, HIGH);
}

void LedHandler::leftLedsOn() {
    digitalWrite(_topLeftLed, HIGH);
    digitalWrite(_bottomLeftLed, HIGH);
}

void LedHandler::rightLedsOn() {
    digitalWrite(_topRightLed, HIGH);
    digitalWrite(_bottomRightLed, HIGH);
}

void LedHandler::ledsOff() {
    digitalWrite(_topLeftLed, LOW);
    digitalWrite(_topRightLed, LOW);
    digitalWrite(_bottomLeftLed, LOW);
    digitalWrite(_bottomRightLed, LOW);
    digitalWrite(_debugLed, LOW);
}

void LedHandler::glow() {
        RGB.brightness(100);
        // Glow colour (Green).
        RGB.color(0, 255, 0);
}

void LedHandler::noglow() {
        RGB.brightness(100);
        RGB.color(0, 0, 0);
}

void LedHandler::rainbowGlow() {
        RGB.brightness(100);
        // TODO: Rainbow the RGB LED...
        RGB.color(0, 255, 255);
}

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
// Constructor for V1 LED handler which sets up the base class
// with the known IO pins or the V1.1 board.
LedHandlerV1::LedHandlerV1() : LedHandler(A0, D3, A5, D7, D7) {
}

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
// Constructor for V2 LED handler which sets up the base class
// with the known IO pins or the V1.2 board.
// NB: V2 hardware supports Neopixels
LedHandlerV2::LedHandlerV2() : LedHandler(A2, D3, A5, D7, D7) {
}

bool LedHandlerV2::init() {
    LedHandler::init();
    
    _strip.begin();
     // Initialize all pixels to 'off'
    _strip.show();
    
    return true;
}

 void LedHandlerV2::glow() {
    // V2 hardware, don't use the tri-color lED on the photon as we have NeoPixels!
    // Set to use a dimish green
    uint32_t color = this->_strip.Color(0, 0, 90);
    for(uint16_t i=0; i<_strip.numPixels(); i++) {
      this->_strip.setPixelColor(i, color);
    }
    this->_strip.show();
}

void LedHandlerV2::noglow() {
    uint32_t color = this->_strip.Color(0, 0, 0);
    for(uint16_t i=0; i<_strip.numPixels(); i++) {
      this->_strip.setPixelColor(i, color);
    }
    this->_strip.show();
}

void LedHandlerV2::rainbowGlow() {
    LedHandlerV2::rainbow(20);
}

void LedHandlerV2::rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<_strip.numPixels(); i++) {
      this->_strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    this->_strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LedHandlerV2::Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return this->_strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return this->_strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return this->_strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


