cmake_minimum_required(VERSION 2.6)
AUX_SOURCE_DIRECTORY(. DIR_VIDEO_SRCS)
ADD_SUBDIRECTORY( json )
ADD_LIBRARY(videoaddress STATIC ${DIR_VIDEO_SRCS})
#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
target_link_libraries(videoaddress json libcurl.so)
INCLUDE_DIRECTORIES(
../include
./include
./json/include
)

