PROJECT(video)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 
ADD_SUBDIRECTORY( src )
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
add_executable (video test_lib.cpp)
SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
INCLUDE_DIRECTORIES(
./include
)
SET(EXECUTABLE_OUTPUT_PATH ./src)
TARGET_LINK_LIBRARIES(video videoaddress libcurl.so)

