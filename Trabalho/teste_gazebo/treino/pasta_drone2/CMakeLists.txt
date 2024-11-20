cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(motor_command)

find_package(gz-transport13 QUIET REQUIRED OPTIONAL_COMPONENTS log)
set(GZ_TRANSPORT_VER ${gz-transport13_VERSION_MAJOR})

include_directories(${CMAKE_BINARY_DIR})
if (EXISTS "${CMAKE_SOURCE_DIR}/motor_command.cpp")
  add_executable(motor_command motor_command.cpp)
  target_link_libraries(motor_command gz-transport${GZ_TRANSPORT_VER}::core)
endif()