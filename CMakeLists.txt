cmake_minimum_required(VERSION 2.8)
project(quasar)

set(CMAKE_CXX_FLAGS "-Werror -Wall -pedantic -std=c++11 -pthread -O0 -ggdb")
message("[INFO] Compilation flags: ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lib)

message("[INFO] Loading sources")
file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
        "src/Decoder.cpp"
        "src/Decoder.h"
        "lib/Exception.cpp"
        "lib/Exception.h")

message("[INFO] Sources: ${SOURCES}")
message("[INFO] Sources loaded!")

add_executable(quasar  src/main.cpp ${SOURCES})

message(STATUS "[INFO] Adding test files")
enable_testing()
find_package(GTest REQUIRED)

add_executable(quasar_test test/main_test.cpp ${SOURCES})
target_link_libraries(quasar_test GTest::GTest GTest::Main)

add_test(NAME quasar_test
        COMMAND quasar_test)
