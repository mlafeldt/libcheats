SET(CMAKE_C_FLAGS "-O2 -Wall -s")

ADD_DEFINITIONS(-D_DEBUG)

SET(SRCS mystring.c cheatlist.c readcheats.c libcheats.c)
ADD_LIBRARY(cheats SHARED ${SRCS})
