#include "application.h"
#include "IrBlaster.h"

///////////////////////////////////////////////////////////////////////////////////
// Base
///////////////////////////////////////////////////////////////////////////////////
// Ctor.
IrBlaster::IrBlaster() {
}


/**
 * @brief Destructor
 */
IrBlaster::~IrBlaster() {
}

/**
 * @brief Base initialiser.
 *
 * @return True if initialized successfully. False otherwise.
 */
bool IrBlaster::init() {
    return true;
}

void IrBlaster::send() {
    // No action by default. Not common to all versions of hardware.
}

///////////////////////////////////////////////////////////////////////////////////
// V1
///////////////////////////////////////////////////////////////////////////////////
// Constructor for V1. No IR LEDs on V1.1 board
// with the known IO pins or the V1.1 board.
IrBlasterV1::IrBlasterV1() : IrBlaster() {
    
}

///////////////////////////////////////////////////////////////////////////////////
// V2
///////////////////////////////////////////////////////////////////////////////////
// Constructor for V2. Two IR LEDs on V2 board
IrBlasterV2::IrBlasterV2() : IrBlaster() {
}

bool IrBlasterV2::init() {
    pinMode(_irLedPin, OUTPUT);
    return true;
}

void IrBlasterV2::send() {
    // No action by default. Not common to all versions of hardware.
    // TODO: Waggle IR LED pin...
}