SET(CMAKE_C_FLAGS "-O2 -Wall")

ADD_DEFINITIONS(-D_DEBUG)

INCLUDE_DIRECTORIES(../include)

ADD_LIBRARY(cheats SHARED cheatlist.c mystring.c parser.c libcheats.c)

INSTALL(TARGETS cheats DESTINATION lib)
INSTALL(FILES ../include/cheatlist.h ../include/libcheats.h DESTINATION include)
