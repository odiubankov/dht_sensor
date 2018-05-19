//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "dht_sensor_11.h"

namespace dht {

int DhtSensor11::readTemperature(const RowDataT& data) const
{
    return static_cast<int>(data[2]);
}


unsigned DhtSensor11::readHumidity(const RowDataT& data) const
{
    return data[0];
}

}
