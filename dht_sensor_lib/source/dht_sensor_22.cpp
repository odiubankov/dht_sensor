//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "dht_sensor_22.h"

namespace dht {

int DhtSensor22::readTemperature(const RowDataT& data) const
{
    auto temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10;
    if (data[2] & 0x80)
      temperature *= -1;

    return temperature;
}


unsigned DhtSensor22::readHumidity(const RowDataT& data) const
{
    return data[0] * 256 + data[1] / 10;
}

}
