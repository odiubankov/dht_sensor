//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#pragma once

#include "dht_sensor.h"

namespace dht {

// Read temperature and humidity values from DHT11 sensor
class DhtSensor11 : public DhtSensor
{
public:
    using DhtSensor::DhtSensor;

protected:
    int readTemperature(const RowDataT& data) const override;
    unsigned readHumidity(const RowDataT& data) const override;
};

}
