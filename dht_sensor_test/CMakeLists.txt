cmake_minimum_required(VERSION 3.5.1)

project(dht_sensor_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${DHT_SOURCE_DIR}/dht_sensor_lib/include)
include_directories(${DHT_SOURCE_DIR}/raspberry_pi_lib/include)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} dht_sensor_lib)
target_link_libraries(${PROJECT_NAME} raspberry_pi_lib)
