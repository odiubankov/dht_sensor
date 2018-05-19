cmake_minimum_required(VERSION 3.5.1)

project(dht_sensor_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)
include_directories(${DHT_SOURCE_DIR}/raspberry_pi_lib/include)

file(GLOB ${PROJECT_NAME}_sources source/*.cpp include/*.h)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} raspberry_pi_lib)
