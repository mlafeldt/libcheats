PROJECT("readcheats" C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_C_FLAGS "-O2 -Wall -s")

SET(SRCS codelist.c codeman.c dllist.c mystring.c gameid.c main.c)

ADD_EXECUTABLE(readcheats ${SRCS})

INSTALL(TARGETS readcheats DESTINATION bin)
