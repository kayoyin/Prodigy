add_library(TestFunctions myrnn.cpp

install (TARGETS TestFunctions DESTINATION bin)
install (FILES TestFunctions.h DESTINATION include)
