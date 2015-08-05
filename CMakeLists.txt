cmake_minimum_required(VERSION 2.8)
project(run_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(ObserverManager gtest/include Tests)
set(SRC_LIST ObserverManager/BaseObserverProtocol.cpp ObserverManager/ObserverManager.cpp Tests/main.cpp Tests/Tests.cpp Tests/SpeedTest.cpp Tests/JimTest.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})

add_subdirectory(gtest)
target_link_libraries(${PROJECT_NAME} gtest)

