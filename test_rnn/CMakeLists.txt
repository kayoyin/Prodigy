cmake_minimum_required (VERSION 2.6)
project (RNN)

# The version number.
set (RNN_VERSION_MAJOR 1)
set (RNN_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/RNNConfig.h.in"
  "${PROJECT_BINARY_DIR}/RNNConfig.h”
  )

# add the binary tree to the search path for include files
# so that we will find RNNConfig.h
include_directories("${PROJECT_BINARY_DIR}")


# should we use our own Test functions?
option (USE_MYTEST
        "Use tutorial provided test implementation" ON)

# add the MathFunctions library?
#
if (USE_MYTEST)
  include_directories ("${PROJECT_SOURCE_DIR}/TestFunctions")
  add_subdirectory (TestFunctions)
  set (EXTRA_LIBS ${EXTRA_LIBS} TestFunctions)
endif (USE_MYTEST)

# add the executable
add_executable(RNN rnn.cpp)
target_link_libraries (RNN TestFunctions)

# add the install targets
install (TARGETS RNN DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/RNNConfig.h"
         DESTINATION include)
