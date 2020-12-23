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
        "src/Server.cpp"
        "src/Server.h"
        "src/HttpController.h"
        "src/TopSecretController.cpp"
        "src/TopSecretController.h"
        "src/TopSecretSplitController.cpp"
        "src/TopSecretSplitController.h"
        "lib/Exception.cpp"
        "lib/Exception.h")

message("[INFO] Sources: ${SOURCES}")

message("[INFO] Adding third-party libraries")
find_package(cpprestsdk REQUIRED)

add_executable(quasar  src/main.cpp ${SOURCES})
target_link_libraries(quasar PRIVATE cpprestsdk::cpprest)

message("[INFO] Adding test files")
enable_testing()
find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
        "test/*.cpp")
add_executable(quasar_test ${TEST_SOURCES} ${SOURCES})
target_link_libraries(quasar_test GTest::GTest GTest::Main cpprestsdk::cpprest)

add_test(NAME quasar_test
        COMMAND quasar_test)
