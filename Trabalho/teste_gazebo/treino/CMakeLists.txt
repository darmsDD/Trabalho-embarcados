cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(fly_drone)

# Find the Gazebo_Transport library
find_package(gz-transport13 QUIET REQUIRED OPTIONAL_COMPONENTS log)
set(GZ_TRANSPORT_VER ${gz-transport13_VERSION_MAJOR})

include_directories(${CMAKE_BINARY_DIR})

if (EXISTS "${CMAKE_SOURCE_DIR}/drone_node.cpp")
  add_executable(drone_node drone_node.cpp)
  target_link_libraries(drone_node gz-transport${GZ_TRANSPORT_VER}::core)
endif()