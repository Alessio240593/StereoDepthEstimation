cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(PointCloud)

set(CMAKE_BUILD_TYPE Release)
set(BIN "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})

link_directories(${PCL_LIBRARY_DIRS})

add_definitions(-DPCL_NO_PRECOMPILE)

add_executable(PointCloud src/pointCloud.cpp)

target_link_libraries(PointCloud ${PCL_LIBRARIES})
target_link_libraries(PointCloud ${Boost_LIBRARIES})
