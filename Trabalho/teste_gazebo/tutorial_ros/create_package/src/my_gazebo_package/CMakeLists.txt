cmake_minimum_required(VERSION 3.8)
project(my_gazebo_package)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()


if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(gz-transport13 QUIET REQUIRED OPTIONAL_COMPONENTS log)
set(GZ_TRANSPORT_VER ${gz-transport13_VERSION_MAJOR})
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(my_gazebo_node src/my_gazebo_node.cpp)
target_link_libraries(my_gazebo_node gz-transport${GZ_TRANSPORT_VER}::core)

install(TARGETS my_gazebo_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
