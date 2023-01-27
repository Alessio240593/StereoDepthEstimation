cmake_minimum_required(VERSION 3.12.2)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# ------------------------------------------------------------------------------
# Setup project.

# Project with not optional CUDA.
project(zed_tests LANGUAGES CUDA CXX C)
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_NAME zed_tests)
set(DEPENDENCIES "")
set(BIN "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN})
include_directories("lib")

set(DEBUG OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Build type not specified: Use Release by default")
  set(DEBUG ON)
endif(NOT CMAKE_BUILD_TYPE)
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Debug mode and logs.
set(DEBUG_LOG_LEVEL 5 CACHE STRING "Logging level")
if(NOT DEBUG_LOG_LEVEL MATCHES "^[0-9]+$")
    message(FATAL_ERROR "DEBUG_LOG_LEVEL must be a number")
endif()

if(DEBUG)
    add_definitions(-DDEBUG=1 -DDEBUG_LOG_LEVEL=${DEBUG_LOG_LEVEL})
    message(STATUS "Enable DEBUG mode with DEBUG_LOG_LEVEL=${DEBUG_LOG_LEVEL}")
else(DEBUG)
    add_definitions(-DDEBUG=0 -DDEBUG_LOG_LEVEL=0)
    message(STATUS "Disable DEBUG mode")
endif(DEBUG)

# Tests and examples
set(BUILD_TEST true CACHE BOOL "Enable tests generation")
set(BUILD_EXAMPLE false CACHE BOOL "Enable examples generation")

# ------------------------------------------------------------------------------

set(DEPENDENCIES ${DEPENDENCIES} -lstdc++fs)

# ------------------------------------------------------------------------------
# Setup CUDA.

# Check if a compiler exists.
if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    message(STATUS "Cuda compiler has been found: ${CMAKE_CUDA_COMPILER}")
else(CMAKE_CUDA_COMPILER)
    message(FATAL_ERROR "Cuda compiler has not been found")
endif()

# Check host compiler.
if(DEFINED ${CMAKE_CUDA_HOST_COMPILER})
    message(STATUS "Cuda host compiler defined: ${CMAKE_CUDA_HOST_COMPILER}")
endif()

# Check correct compiler id.
if(CMAKE_CUDA_COMPILER_ID STREQUAL "NVIDIA")
    message(STATUS "Cuda compiler id match NVIDIA")
    message(STATUS "Compute Capability: ${CMAKE_CUDA_ARCHITECTURES}")
else(CMAKE_CUDA_COMPILER_ID STREQUAL "NVIDIA")
    message(FATAL_ERROR "Cuda compiler id doesn't match")
endif()

message(STATUS 
        "CMake setup project succeeds with NVIDIA "
        "v${CMAKE_CUDA_COMPILER_VERSION}")

# ------------------------------------------------------------------------------
# Setup OpenCV.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(DEPENDENCIES ${DEPENDENCIES} ${OpenCV_LIBS})
list(APPEND CUDA_NVCC_FLAGS ${OpenCV_COMPUTE_CAPABILITIES})
message(STATUS "OpenCV include directories: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV linking library dependencies: ${OpenCV_LIBS}")
message(STATUS "OpenCV compilation compute capability: ${OpenCV_COMPUTE_CAPABILITIES}")
message(STATUS "OpenCV compilation with CUDA version: ${OpenCV_CUDA_VERSION}")
message(STATUS "OpenCV_USE_CUBLAS: ${OpenCV_USE_CUBLAS}")
message(STATUS "OpenCV_USE_CUFFT: ${OpenCV_USE_CUFFT}")
message(STATUS "OpenCV_USE_NVCUVID: ${OpenCV_USE_NVCUVID}")

# ------------------------------------------------------------------------------
# Setup ZED SDK.
option(LINK_SHARED_ZED "Link with the ZED SDK shared executable" ON)

if (NOT LINK_SHARED_ZED AND MSVC)
    message(FATAL_ERROR "LINK_SHARED_ZED OFF : ZED SDK static libraries not available on Windows")
endif()

find_package(ZED 3 REQUIRED)
find_package(CUDA ${ZED_CUDA_VERSION} EXACT REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${ZED_INCLUDE_DIRS})
if (LINK_SHARED_ZED)
    set(ZED_LIBS ${ZED_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY})
else()
    set(ZED_LIBS ${ZED_STATIC_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_LIBRARY})
endif()

set(DEPENDENCIES ${DEPENDENCIES} ${ZED_LIBS})


# ------------------------------------------------------------------------------
# Set the C++ compiler options.
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i686")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-conversion")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-nonliteral")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-y2k")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimport")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winline")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winvalid-pch")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-field-initializers")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-format-attribute")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-include-dirs")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpacked")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstack-protector")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wstrict-aliasing=2")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-enum")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-function")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-label")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-parameter")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-value")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-variable")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wvariadic-macros")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wwrite-strings")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-compare")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")

# Thread
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# ------------------------------------------------------------------------------
# Set the C compiler options.
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdinc")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=i686")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++14")
# set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -Wall")
# set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -Wpedantic")
# set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -pedantic-errors")
# set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -Wextra")
# set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -Werror")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconversion")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-qual")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdisabled-optimization")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfloat-equal")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfloat-conversion")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat=2")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-nonliteral")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-security")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-y2k")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wimport")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winit-self")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winline")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winvalid-pch")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-field-initializers")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-include-dirs")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpacked")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wredundant-decls")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstack-protector")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-aliasing=2")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch-default")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch-enum")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunreachable-code")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-function")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-label")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-parameter")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-value")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-variable")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wvariadic-macros")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wwrite-strings")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-compare")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-conversion")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4")

# ------------------------------------------------------------------------------
# Set the CUDA compiler options.
set(CA ${CMAKE_CUDA_ARCHITECTURES}) # Compute Capability
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${CA},code=sm_${CA}")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${CA},code=compute_${CA}")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O1")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O2")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O3")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -w")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -std=c++14")

# ------------------------------------------------------------------------------
# Add the includes.
#include_directories(
#    include
#)

# ------------------------------------------------------------------------------
# Add the source files.
set(CXX_SOURCE_FILES
    main
)

set(CUDA_SOURCE_FILES
)

# ------------------------------------------------------------------------------
# Add the library.
set(LIBRARY_NAME ${PROJECT_NAME})

set(SOURCES
    ${CXX_SOURCE_FILES}
    ${CUDA_SOURCE_FILES}
)

foreach(SOURCE ${SOURCES})
    add_executable(${SOURCE}
        src/${SOURCE}.cpp
    )

    target_link_libraries(${SOURCE} ${DEPENDENCIES})
endforeach()

