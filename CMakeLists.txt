# Partially taken from https://github.com/mlpack/models
#
project(PRODIGY)
cmake_minimum_required(VERSION 3.0)

# The version number.
set (PRODIGY_VERSION_MAJOR 1)
set (PRODIGY_VERSION_MINOR 0)

option(FORCE_CXX11
    "Don't check that the compiler supports C++11, just assume it.  Make sure to specify any necessary flag to enable C++11 as part of CXXFLAGS."
    ON)
option(USE_OPENMP "If available, use OpenMP for parallelization." ON)
enable_testing()

# Set required standard to C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the CFLAGS and CXXFLAGS.
# Set the CFLAGS and CXXFLAGS depending on the options the user specified.
# Only GCC-like compilers support -Wextra, and other compilers give tons of
# output for -Wall, so only -Wall and -Wextra on GCC.
if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -ftemplate-depth=1000 -DBOOST_MATH_DISABLE_FLOAT128")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -DBOOST_MATH_DISABLE_FLOAT128")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -g -O0 -DBOOST_MATH_DISABLE_FLOAT128") 
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
add_executable(train ${PROJECT_SOURCE_DIR}/src/train.cpp)
add_executable(compose ${PROJECT_SOURCE_DIR}/src/compose.cpp)
add_executable(composegui ${PROJECT_SOURCE_DIR}/src/composegui.cpp)


# Set the libraries to link against
target_link_libraries(train
  ${MLPACK_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${Boost_LIBRARIES})
  
target_link_libraries(compose
  ${MLPACK_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${Boost_LIBRARIES})

target_link_libraries(composegui
  ${MLPACK_LIBRARIES}
  ${ARMADILLO_LIBRARIES}
  ${Boost_LIBRARIES})
