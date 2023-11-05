# This is the main CMake configuration file for the hwinfo project.
# hwinfo is a C++ project designed to gather various system information,
# including memory, CPU, and operating system details. The project can optionally
# use OpenCL to get GPU information, build documentation using Doxygen, and
# build its own test suite and sample programs.

# Specify the minimum version of CMake required to build this project.
cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

# Define the project name, version, and the languages used (C++ in this case).
project(hwinfo VERSION 1.0.0 LANGUAGES CXX)

# Define options that can be turned on or off to control project build features.
option(HWINFO_USE_OCL "Use OpenCL to get GPU information" OFF)
option(HWINFO_CREATE_DOCS "Build library documentation (requires Doxygen and Graphviz/Dot to be installed)" ${PROJECT_IS_TOP_LEVEL})
option(HWINFO_BUILD_TESTS "Build library tests" ${PROJECT_IS_TOP_LEVEL})
option(HWINFO_BUILD_SAMPLES "Build sample programs" ${PROJECT_IS_TOP_LEVEL})

# Set the C++ standard to use and enforce it as required.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Suppress CMake warning about unused C compiler during C++-only projects.
set(HWINFO_IGNORE_ME ${CMAKE_C_COMPILER})

# Conditionally configure the project to use OpenCL based on the HWINFO_USE_OCL option.
#if (HWINFO_USE_OCL)
#    # Prevent the OpenCL SDK samples from being built.
#    set(OPENCL_SDK_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
#    # Include directories for OpenCL headers.
#    include_directories(external/miss-opencl/include)
#    include_directories(external/miss-opencl/external/OpenCL/external/OpenCL-CLHPP/include)
#    # Add the miss-opencl subdirectory only if it's not already defined as a target.
#    if (NOT TARGET miss-opencl_static)
#        add_subdirectory(external/miss-opencl)
#    endif ()
#    # Define compile-time macros for OpenCL usage.
#    add_compile_definitions(USE_OCL)
#    add_compile_definitions(NOMINMAX)
#endif ()

# Create an INTERFACE library target called HWinfo to be used by other targets.
add_library(HWinfo INTERFACE)
# Create an alias for the HWinfo library to ensure consistent usage.
add_library(${PROJECT_NAME}::HWinfo ALIAS HWinfo)
# Specify the include directories for the HWinfo interface library.
target_include_directories(HWinfo INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/include)

# If building on Windows, link the WBEM UUID library which is required for some system queries.
if (WIN32)
    target_link_libraries(HWinfo INTERFACE "wbemuuid")
endif ()

# If OpenCL is used, link the static OpenCL library to HWinfo.
#if (HWINFO_USE_OCL)
#    target_link_libraries(HWinfo INTERFACE miss-opencl_static)
#endif ()

# If building on Linux, copy the pci.ids file to the user's home directory.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    file(COPY "scripts/pci.ids" DESTINATION "$ENV{HOME}/.hwinfo/")
endif ()

# If the option to build samples is turned on, add the samples subdirectory to the build.
if (HWINFO_BUILD_SAMPLES)
    add_subdirectory(examples)
endif()
