PROJECT("libcheats" C)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(CMAKE_C_FLAGS "-O2 -Wall -s")

ADD_DEFINITIONS(-D_DEBUG)

SET(SRCS mystring.c cheatlist.c readcheats.c libcheats.c)
ADD_LIBRARY(cheats SHARED ${SRCS})

ENABLE_TESTING()

ADD_EXECUTABLE(test1 test1.c)
TARGET_LINK_LIBRARIES(test1 cheats)
ADD_TEST(cheats_read_file ${EXECUTABLE_OUTPUT_PATH}/test1 ${CMAKE_SOURCE_DIR}/cheats.txt)

ADD_EXECUTABLE(test2 test2.c)
TARGET_LINK_LIBRARIES(test2 cheats)
ADD_TEST(cheats_read_buf ${EXECUTABLE_OUTPUT_PATH}/test2)
