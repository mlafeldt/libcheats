PROJECT("readcheats" C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_C_FLAGS "-O2 -Wall -s")

ADD_DEFINITIONS(-D_DEBUG)

SET(SRCS cheatlist.c mystring.c readcheats.c main.c)

ADD_EXECUTABLE(readcheats ${SRCS})

INSTALL(TARGETS readcheats DESTINATION bin)
