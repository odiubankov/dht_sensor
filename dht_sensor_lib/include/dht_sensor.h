//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#include "memory_mapped_io.h"
#include "gpio_pin.h"

#include <array>

namespace dht {

// Base class for DhtSensor11 and DhtSensor22
class DhtSensor
{
public:
    DhtSensor(raspby::MemoryMappedIO memoryMappedIO, raspby::GpioPin pin);
    virtual ~DhtSensor() = default;
    DhtSensor(const DhtSensor&) = default;
    DhtSensor& operator=(const DhtSensor&) = default;
    DhtSensor& operator=(DhtSensor&&) = default;

    // Read current humidity and temperature values
    void read();

    // Get latest read humidity value
    unsigned getHumidity() const { return humidity_; }
    // Get latest read temperature value
    int getTemperature() const { return temperature_; }

protected:
    // Row data that represents temperature and humidity values
    using RowDataT = std::array<uint8_t, 5>;
    // Get temperature value from row data
    virtual int readTemperature(const RowDataT& data) const = 0;
    // Get humidity value from row data
    virtual unsigned readHumidity(const RowDataT& data) const = 0;

private:
    // Actual communication with dht sensor
    void readPulses();
    // Interpreatation of pulses data that was recieved from dht sensor
    void interpret();
    // Reading pulses from sensor
    void storePulseCounts();
    // Wait until pin is set to low
    void waitForPinLow() const;
    // Get average pulses count value for the low value
    uint32_t getThreshold() const;

    raspby::MemoryMappedIO memoryMappedIO_;
    raspby::GpioPin pin_;

    // One pulse data
    struct DhtPulse
    {
        int lowCnt_{0};
        int highCnt_{0};
    };

    // Number of bit pulses to expect from the DHT.  Note that this is 41 because
    // the first pulse is a constant 50 microsecond pulse, with 40 pulses to represent
    // the data afterwards.
    static constexpr int DHT_PULSES_CNT = 41;
    std::array<DhtPulse, DHT_PULSES_CNT> pulses_;

    unsigned humidity_{0};
    int temperature_{0};
};

}
