# Partially taken from https://github.com/mlpack/models
#
project(NNSAMPLE)
cmake_minimum_required(VERSION 3.0)

# The version number.
set (NNSAMPLE_VERSION_MAJOR 1)
set (NNSAMPLE_VERSION_MINOR 0)

option(FORCE_CXX11
    "Don't check that the compiler supports C++11, just assume it.  Make sure to specify any necessary flag to enable C++11 as part of CXXFLAGS."
    OFF)

# If we are not forcing C++11 support, check that the compiler supports C++11
# and enable it.
if (NOT FORCE_CXX11)
  include(cmake/CXX11.cmake)
  check_for_cxx11_compiler(HAS_CXX11)

  if(NOT HAS_CXX11)
    message(FATAL_ERROR "No C++11 compiler available!")
  endif()
  enable_cxx11()
endif ()

# Set the CFLAGS and CXXFLAGS.
# Set the CFLAGS and CXXFLAGS depending on the options the user specified.
# Only GCC-like compilers support -Wextra, and other compilers give tons of
# output for -Wall, so only -Wall and -Wextra on GCC.
if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -ftemplate-depth=1000")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -g -O0") 
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


#############################################################################
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_BINARY_DIR}/src)


# Check for the mlpack library
find_package (MLPACK REQUIRED)
find_package (ARMADILLO REQUIRED)

find_package(Boost 
    COMPONENTS
      program_options
      unit_test_framework
      serialization
    REQUIRED
)

find_package(OpenMP)

# Include the headers of the libraries
set(MODELS_INCLUDE_DIRS ${MLPACK_INCLUDE_DIR}
                        ${ARMADILLO_INCLUDE_DIR}
                        ${Boost_INCLUDE_DIRS})
set(MODELS_LIBRARIES ${MLPACK_LIBRARY}
                     ${ARMADILLO_LIBRARIES}
                     ${Boost_LIBRARIES})
include_directories(${MODELS_INCLUDE_DIRS})

add_definitions(-DBOOST_TEST_DYN_LINK)

message("Found ${MODELS_INCLUDE_DIRS}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# add the executable
add_executable(nnsample ${PROJECT_SOURCE_DIR}/src/main.cpp)
# Set the libraries to link against
target_link_libraries(nnsample
  ${MLPACK_LIBRARIES}
  ${ARMADILLO_LIBRARIES})

