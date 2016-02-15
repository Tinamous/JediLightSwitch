#include "application.h"
#include "PowerManagement.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
// Ctor.
PowerManagement::PowerManagement() {
}

/**
 * @brief Destructor
 */
PowerManagement::~PowerManagement() {
}

/**
 * @brief Initializes power management
 *
 * @return True if initialized successfully. False otherwise.
 */
bool PowerManagement::init() {
}

void PowerManagement::appendSenML(SenMLBuilder* builder) {
    // No action for base & V1 classes.
}

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
PowerManagementV1::PowerManagementV1() {
}

void PowerManagementV1::readPowerState(bool publish) {
    // Nothing to implement.
}

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
PowerManagementV2::PowerManagementV2() {
}

bool PowerManagementV2::init() {
    pinMode(_batteryMonitorAnalogPin, INPUT);
    pinMode(_chargeStatusPin, INPUT);
    
    _batteryMonitor.reset();
    _batteryMonitor.quickStart();
        
    return true;
}

void PowerManagementV2::readPowerState(bool publish) {
    _cellVoltage = _batteryMonitor.getVCell();
    _stateOfCharge = _batteryMonitor.getSoC();
    
    // Battery charger status
    _chargeState = digitalRead(_chargeStatusPin);
    
    // Battery ADC value
    _batteryAdc = analogRead(_batteryMonitorAnalogPin);
    
    // Convert ADC value (0.8mv per ADC, 1/2 voltage divider, millivolrs -> volts)
    _batteryVoltageAnalog = ( (_batteryAdc * 0.8F) * 2.0F / 1000);
    
    if (publish) {
        Particle.publish("senml", "{e:[{'n':'ps-voltage','v':'" + String(_cellVoltage) + "'},{'n':'ps-soc','v':'" + String(_stateOfCharge) + "'},{'n':'charge-status','v':'" + String(_chargeState) + "'},{'n':'battery-adc','v':'" + String(_batteryAdc) + "'} ]}", 60, PRIVATE);
    }
}


void PowerManagementV2::appendSenML(SenMLBuilder* builder) {
    //Particle.publish("senml", "{e:[{'n':'ps-voltage','v':'" + String(_cellVoltage) + "'},{'n':'ps-soc','v':'" + String(_stateOfCharge) + "'},{'n':'charge-status','v':'" + String(_chargeState) + "'},{'n':'battery-adc','v':'" + String(_batteryAdc) + "'} ]}", 60, PRIVATE);
    
    builder->add("ps-volts", _cellVoltage);
    builder->add("ps-soc", _stateOfCharge);
    builder->add("charging", _chargeState);
    builder->add("batt-adc", _batteryAdc);
    builder->add("batt-volts", _batteryVoltageAnalog);
}
