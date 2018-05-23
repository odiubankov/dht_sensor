//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "memory_mapped_io.h"
#include "dht_sensor_11.h"
#include "gpio_pin.h"

#include <iostream>
#include <thread>

namespace {

// GPIO pin connected to DHT11 sensor
const unsigned CONNECTED_PIN = 4;
// Number of times to try to read the temperature and humidity values
const unsigned READING_CNT = 10;
// Timeout in seconds between readings
const unsigned READING_TIMEOUT_SECONDS = 2;

}

int main()
{
    std::cout << "Raspberry Pi DHT11/DHT22 temperature/humidity test" << std::endl;

    raspby::MemoryMappedIO memoryMappedIO;
    auto pin = raspby::GpioPin::create<CONNECTED_PIN>();
    dht::DhtSensor11 sensor{memoryMappedIO, pin};
    for (int i = 0; i < READING_CNT; ++i)
    {
        try
        {
            sensor.read();
            std::cout << "humidity " << sensor.getHumidity() << " temperature " << sensor.getTemperature() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds{READING_TIMEOUT_SECONDS});
        }
        catch(const std::runtime_error& error)
        {
            std::cout << "Exception on reading: " << error.what() << std::endl;
        }
    }

    return 0;
}
