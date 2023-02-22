CC = g++
DEBUG = -g -DDEBUG
CFLAGS = -Wall -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)

SRC = \
../src/async_writer.cpp \
../src/compute.cpp \

INCLUDE_HEADER_LOCATION=\
-I../src/

LINK_LIB_LOCATION=\

LINK_LIB_NAME=\
-ltbb \
-pthread \

compile:
	${CC} -o test.out ${CFLAGS} main.cpp ${SRC} ${INCLUDE_HEADER_LOCATION} ${LINK_LIB_LOCATION} ${LINK_LIB_NAME}

clean:
	rm -f test_async_result.txt
	rm -f test_async_id.txt
	rm -f test.out


