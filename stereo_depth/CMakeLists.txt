cmake_minimum_required(VERSION 3.12.2)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# ------------------------------------------------------------------------------
# Setup project.

project(stereodepth LANGUAGES CUDA CXX)
set(CMAKE_CXX_STANDARD 14)
set(DEPENDENCIES "")
set(INCLUDES "")

set(DEBUG OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default")
  set(DEBUG OFF)
endif(NOT CMAKE_BUILD_TYPE)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DEBUG ON)
endif()
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
set(BUILD_TEST OFF CACHE BOOL "Enable tests generation")
set(BUILD_EXAMPLE ON CACHE BOOL "Enable examples generation")


# ------------------------------------------------------------------------------
# Setup dependencies.
include(cmake/GPUConfig.cmake)

# Test.
enable_testing()
include(CTest)

# Check if CUDA is available.
include(CheckLanguage)
check_language(CUDA)

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
# Setup OpenMP.
set(OPENMP ON CACHE BOOL "Use OpenMP library")
add_definitions(-DOPENMP=${OPENMP})
if (OPENMP)
    find_package(OpenMP REQUIRED)
    if (OPENMP_FOUND)
        add_compile_options(${OpenMP_CXX_FLAGS})
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler ${OpenMP_CXX_FLAGS}")
        message(STATUS "OpenMP: enabled")
    else (OPENMP_FOUND)
        message(FATAL_ERROR "OpenMP not found")
    endif ()
endif ()

# ------------------------------------------------------------------------------
# Setup OpenCV.
find_package(OpenCV REQUIRED)
set(INCLUDES ${INCLUDES} ${OpenCV_INCLUDE_DIRS})
set(DEPENDENCIES ${DEPENDENCIES} ${OpenCV_LIBS})
list(APPEND CUDA_NVCC_FLAGS ${OpenCV_COMPUTE_CAPABILITIES})
message(STATUS "OpenCV include directories: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV linking library dependecies: ${OpenCV_LIBS}")
message(STATUS "OpenCV compilation compute capability: ${OpenCV_COMPUTE_CAPABILITIES}")
message(STATUS "OpenCV compilation with CUDA version: ${OpenCV_CUDA_VERSION}")
message(STATUS "OpenCV_USE_CUBLAS: ${OpenCV_USE_CUBLAS}")
message(STATUS "OpenCV_USE_CUFFT: ${OpenCV_USE_CUFFT}")
message(STATUS "OpenCV_USE_NVCUVID: ${OpenCV_USE_NVCUVID}")

# ------------------------------------------------------------------------------
# Set the C++ compiler options.
# add_compile_options(-O1)
# add_compile_options(-O2)
add_compile_options(-O3)
add_compile_options(-g3)
add_compile_options(-ggdb)
# add_compile_options(-fomit-frame-pointer)
# add_compile_options(-fno-builtin)
# add_compile_options(-fno-stack-protector)
# add_compile_options(-Wall)
# add_compile_options(-Werror)
# add_compile_options(-march=i686)
# add_compile_options(-m32)
# add_compile_options(-Wpedantic)
# add_compile_options(-pedantic-errors)
# add_compile_options(-Wextra)
# add_compile_options(-Wconversion)
# add_compile_options(-Wcast-align)
# add_compile_options(-Wcast-qual)
# add_compile_options(-Wdisabled-optimization)
# add_compile_options(-Wfloat-equal)
# add_compile_options(-Wfloat-conversion)
# add_compile_options(-Wformat=2)
# add_compile_options(-Wformat-nonliteral)
# add_compile_options(-Wformat-security)
# add_compile_options(-Wformat-y2k)
# add_compile_options(-Wimport)
# add_compile_options(-Winit-self)
# add_compile_options(-Winline)
# add_compile_options(-Winvalid-pch)
# add_compile_options(-Wno-long-long)
# add_compile_options(-Wmissing-field-initializers)
# add_compile_options(-Wmissing-format-attribute)
# add_compile_options(-Wmissing-include-dirs)
# add_compile_options(-Wpacked)
# add_compile_options(-Wpointer-arith)
# add_compile_options(-Wredundant-decls)
# add_compile_options(-Wshadow)
# add_compile_options(-Wstack-protector)
# add_compile_options(-Wstrict-aliasing=2)
# add_compile_options(-Wswitch)
# add_compile_options(-Wswitch-default)
# add_compile_options(-Wswitch-enum)
# add_compile_options(-Wunreachable-code)
# add_compile_options(-Wunused)
# add_compile_options(-Wunused-function)
# add_compile_options(-Wunused-label)
# add_compile_options(-Wunused-parameter)
# add_compile_options(-Wunused-value)
# add_compile_options(-Wunused-variable)
# add_compile_options(-Wvariadic-macros)
# add_compile_options(-Wwrite-strings)
# add_compile_options(-Wsign-compare)
# add_compile_options(-Wsign-conversion)
# add_compile_options(-Wuninitialized)
# add_compile_options(-fmessage-length=0)

# ------------------------------------------------------------------------------
# Set the CUDA compiler options.
set(CA ${CMAKE_CUDA_ARCHITECTURES}) # Compute Capability
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${CA},code=sm_${CA}")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${CA},code=compute_${CA}")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O1")
# set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O2")
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O3")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -w")

# ------------------------------------------------------------------------------
# Add the library.
set(LIBRARY_NAME ${PROJECT_NAME})

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
#add_library(${LIBRARY_NAME}
#    $<TARGET_OBJECTS:${LIBRARY_NAME}-core>
#)
#target_link_libraries(${LIBRARY_NAME} ${DEPENDENCIES})

# ------------------------------------------------------------------------------
# Add the tests.
if (BUILD_TEST)
    add_subdirectory(test)
endif()

# ------------------------------------------------------------------------------
#Add the examples.
#if (BUILD_EXAMPLE)
#    add_subdirectory(example)
#endif()