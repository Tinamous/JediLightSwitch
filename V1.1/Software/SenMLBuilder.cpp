#include "application.h"
#include "SenMLBuilder.h"

// Ctor.
SenMLBuilder::SenMLBuilder() {
    // Maximum size of a published message is 255 characters.
    _senml.reserve(255);
}

/**
 * @brief Destructor
 */
SenMLBuilder::~SenMLBuilder() {
}

/**
 * @brief Start a new SenML document.
 *
 */
void SenMLBuilder::begin() {
    // Clear out the old string buffer
    _senml = "{e:[";
    _first = true;
    _terminated = false;
}

void SenMLBuilder::prependComma() {
    // Prepend a comma to anything but the first measurement
    // to seperate the json objects in the senml.e array.
    if (!_first) {
        _senml += ",";
    }
    _first = false;
}

void SenMLBuilder::add(String fieldName, String value) {
    // {'n':'Temperature','v':'22'}
    prependComma();
    _senml += "{'n':'" + fieldName + "','v':'" + value + "'},";
    
}

void SenMLBuilder::add(String fieldName, int value) {
    // {'n':'Temperature','v':'22'}
    prependComma();
    _senml += "{'n':'" + fieldName + "','v':'" + String(value) + "'}";
}

void SenMLBuilder::add(String fieldName, float value) {
    prependComma();
    _senml += "{'n':'" + fieldName + "','v':'" + String(value) + "'}";
}

void SenMLBuilder::add(String fieldName, uint16_t value) {
    prependComma();
    _senml += "{'n':'" + fieldName + "','v':'" + String(value) + "'}";
}

// Complete the document and convert it to a string
// This method may be called repeatedly.
String SenMLBuilder::toString() {
    // Add each or the recorded sensor measurements
    // {'n':'Temperature','v':'" + String(environmentSensors->getTemperature()) + "'}
    //
    if (!_terminated) {
        _senml += "]}";
        _terminated = true;
    }
    
    return _senml;
}

// Publish the senml document
// private. 60s ttl, name="senml"
// This method may be called repeatedly in case of failure.
bool SenMLBuilder::publish() {
    return Particle.publish("senml", toString(), 60, PRIVATE);
}