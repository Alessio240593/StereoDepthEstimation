

cmake_minimum_required(VERSION 3.12.2)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project(StereoDepth VERSION 0.1)

set(DEPENDENCIES "")
# set(INCLUDES "")

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
set(BUILD_TEST ON CACHE BOOL "Enable tests generation")
set(BUILD_EXAMPLE ON CACHE BOOL "Enable examples generation")

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MACOSX_RPATH 1)
set(BUILD_SHARED_LIBS ON)
if(WIN32)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(LIBRARY_KIND STATIC)
else()
    set(LIBRARY_KIND SHARED)
endif()

if(NOT WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # add_compile_options(-O1)
        # add_compile_options(-O2)
        add_compile_options(-O3)
        add_compile_options(-g3)
        add_compile_options(-ggdb)
        add_compile_options(-w)
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
    endif()
endif()

enable_testing()
add_custom_target(tests)
include(CTest)

set(DEPENDENCIES ${DEPENDENCIES} -lstdc++fs)

# ------------------------------------------------------------------------------
# Setup CUDA.

include(cmake/GPUConfig.cmake)

include(CheckLanguage)
check_language(CUDA)

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

if(NOT WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CA ${CMAKE_CUDA_ARCHITECTURES}) # Compute Capability
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode=arch=compute_${CA},code=sm_${CA}")
    endif()
endif()
# ------------------------------------------------------------------------------
# Setup OpenMP.
set(OPENMP ON CACHE BOOL "Use OpenMP library")
add_definitions(-DOPENMP=${OPENMP})
if (OPENMP)
    find_package(OpenMP REQUIRED)
    if (OPENMP_FOUND)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
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
# Configure library.

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

if (BUILD_TEST)
    add_subdirectory(test)
endif()

if (BUILD_EXAMPLE)
   add_subdirectory(example)
endif()

#if (BUILD_BENCHMARK)
    add_subdirectory(benchmark)
#endif()

add_library(stereodepth ${LIBRARY_KIND}
    $<TARGET_OBJECTS:stereodepth-core>
)

if (EXISTS "/etc/nv_tegra_release")
    target_link_libraries(stereodepth -lstdc++fs)
endif()
