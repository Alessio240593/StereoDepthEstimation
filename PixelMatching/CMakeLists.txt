cmake_minimum_required(VERSION 2.8.12)
project( CorssCorrelation )

set(BIN "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})
set(SOURCES src/main.cpp)

enable_testing()

add_subdirectory(src)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable( CrossCorrelation ${SOURCES})
target_link_libraries( CrossCorrelation lib)
