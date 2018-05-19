//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "dht_sensor.h"
#include "priority_booster.h"
#include "timing.h"

#include <algorithm>
#include <iterator>
#include <thread>

namespace {

// This is the only processor specific magic value, the maximum amount of time to
// spin in a loop before bailing out and considering the read a timeout.  This should
// be a high value, but if you're running on a much faster platform than a Raspberry
// Pi or Beaglebone Black then it might need to be increased.
const int DHT_MAXCOUNT = 64000;

}


namespace dht {

DhtSensor::DhtSensor(raspby::MemoryMappedIO memoryMappedIO, raspby::GpioPin pin)
    : memoryMappedIO_(memoryMappedIO)
    , pin_{ pin }
{}


void DhtSensor::read()
{
    readPulses();
    interpret();
}


uint32_t DhtSensor::getThreshold() const
{
    // Ignore the first two readings because they are a constant 80 microsecond pulse.
    std::vector<int> lowValues;
    lowValues.reserve(pulses_.size() - 1);
    std::transform(begin(pulses_) + 1, end(pulses_), back_inserter(lowValues),
                   [](const DhtPulse& p) { return p.lowCnt_; });
    return std::accumulate(begin(lowValues), end(lowValues), 0) / lowValues.size();
}


void DhtSensor::interpret()
{
    auto threshold = getThreshold();
    // Interpret each high pulse as a 0 or 1 by comparing it to the 50us reference.
    // If the count is less than 50us it must be a ~28us 0 pulse, and if it's higher
    // then it must be a ~70us 1 pulse.
    RowDataT data;
    auto firstPulse = begin(pulses_) + 1;
    for (auto pulseIt = firstPulse; pulseIt != end(pulses_); ++pulseIt)
    {
      int index = std::distance(firstPulse, pulseIt) / 8;
      data[index] <<= 1;
      if (pulseIt->highCnt_ >= threshold)
          data[index] |= 1;// one bit for long pulse.
      //else
          //zero bit for short pulse.
    }

    // Verify checksum of received data.
    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
        throw std::runtime_error{"Invalid checksum"};

    temperature_ = readTemperature(data);
    humidity_ = readHumidity(data);
}


void DhtSensor::readPulses()
{
    memoryMappedIO_.setOutput(pin_);
    raspby::PriorityBooster priorityBooster;

    memoryMappedIO_.setHigh(pin_);
    std::this_thread::sleep_for(std::chrono::milliseconds{500});

    memoryMappedIO_.setLow(pin_);
    raspby::busyWaitFor(std::chrono::milliseconds{20});

    waitForPinLow();
    storePulseCounts();
}


void DhtSensor::waitForPinLow() const
{
    memoryMappedIO_.setInput(pin_);
    // Need a very short delay before reading pins or else value is sometimes still low.
    raspby::shortBusyWait();

    uint32_t count = 0;
    while (memoryMappedIO_.input(pin_))
      if (++count >= DHT_MAXCOUNT)
          throw std::runtime_error("Timeout in wait for pin low");
}


void DhtSensor::storePulseCounts()
{
    pulses_.fill(DhtPulse{});
    // Record pulse widths for the expected result bits.
    for (auto& dhtPulse : pulses_)
    {
      // Count how long pin is low
      while (!memoryMappedIO_.input(pin_))
        if (++dhtPulse.lowCnt_ >= DHT_MAXCOUNT)
          throw std::runtime_error("Timeout in storing pin low count");

      // Count how long pin is high
      while (memoryMappedIO_.input(pin_))
        if (++dhtPulse.highCnt_ >= DHT_MAXCOUNT)
          throw std::runtime_error("Timeout in storing pin high count");
    }
}

}
