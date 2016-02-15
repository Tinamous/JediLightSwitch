#ifndef PowerManagement_H
#define PowerManagement_H

#include "PowerShield/PowerShield.h"
#include "SenMLBuilder.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
// Responsible for reading the LiPol battery voltage and charge.
class PowerManagement {
public:
    PowerManagement();
    ~PowerManagement();
    
    virtual bool init();
    // Pure virtual - must be overridden
    virtual void readPowerState(bool publish) = 0;
    
    // Append sensor measurements to SenML Builder
    // for publication.
    virtual void appendSenML(SenMLBuilder* builder);

protected:

private:
    
};

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
// V1.1 board does not support power management
class PowerManagementV1 : public PowerManagement {
public:
    PowerManagementV1();
    void readPowerState(bool publish) override;
};

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
// V1.2 board has battery/charger and monitor.
class PowerManagementV2 : public PowerManagement {
public:
    PowerManagementV2();
    bool init() override;
    void readPowerState(bool publish) override;
    void appendSenML(SenMLBuilder* builder) override;
private:
    PowerShield _batteryMonitor;
    // Battery is connected to ADC via potential divider on pin...
    int _batteryMonitorAnalogPin = A4;
    int _chargeStatusPin = A3;
    float _cellVoltage;
    float _stateOfCharge;
    int _chargeState;
    // Battery voltage as measured via ADC
    int _batteryAdc;
    float _batteryVoltageAnalog;
};

#endif