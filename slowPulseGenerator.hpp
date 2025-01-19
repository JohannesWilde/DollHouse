#ifndef SLOW_PULSE_GENERATOR_HPP
#define SLOW_PULSE_GENERATOR_HPP

#include "ArduinoDrivers/avrpin.hpp"

#include <Arduino.h>

#include <stdint.h>

/**
 * @brief For keeping any powerbank [even those that don't support
 * trickle-charging] turned on, turn on a pin every ~12 seconds [assuming
 * 20 Hz update rate] and off after 10 milliseconds.
 * This in turn shall trigger a mosfet, which grounds a 10 Ohm resistor
 * and thus results in a temporary current of I = 5V / 10R = 0.5A, which
 * is more than the typically required 100mA.
 */
template<typename AvrPin_>
class SlowPulseGenerator
{
public:
    static uint16_t constexpr millisecondsToSleep = 10;

    static void initialize()
    {
        AvrPin_::setType(AvrInputOutput::PinType::OutputLow);
        updateTimesSinceLastToggle_ = 0;
    }

    static void deinitialize()
    {
        AvrPin_::setType(AvrInputOutput::PinType::Input);
    }

    static bool update()
    {
        ++updateTimesSinceLastToggle_;

        bool const pulseNow = (250 <= updateTimesSinceLastToggle_);

        if (pulseNow)
        {
            // Pull up pin for millisecondsToSleep.
            AvrPin_::setPort();
            delay(millisecondsToSleep);
            AvrPin_::clearPort();

            updateTimesSinceLastToggle_ = 0;
        }

        return pulseNow;
    }

private:
    static uint8_t updateTimesSinceLastToggle_; // in periods and not ms.

};


template<typename AvrPin_>
uint8_t SlowPulseGenerator<AvrPin_>::updateTimesSinceLastToggle_ = 0;

#endif // SLOW_PULSE_GENERATOR_HPP
