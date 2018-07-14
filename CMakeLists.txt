cmake_minimum_required (VERSION 3.5.1)
project (dht)

set (DHT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory (raspberry_pi_lib)
add_subdirectory (dht_sensor_lib)
add_subdirectory (dht_sensor_test)
