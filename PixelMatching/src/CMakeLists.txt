cmake_minimum_required(VERSION 2.8.12)
project( CorssCorrelation )

find_package( OpenCV REQUIRED)

add_library(lib CrossCorrelation.cpp)
target_link_libraries( lib ${OpenCV_LIBS} )
target_include_directories (lib PUBLIC ${CMAKE_SOURCE_DIR}/include)