cmake_minimum_required(VERSION 2.8)

project( DisplayMap )
set(BIN "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisparityMap src/disparityMap.cpp )
target_link_libraries( DisparityMap ${OpenCV_LIBS} )
