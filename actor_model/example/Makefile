OBJS = test_tensor1.o test_tensor2.o test_tensor3.o test_tensor4.o
CC = mpic++
DEBUG = -g -DDEBUG
CFLAGS = -Wall -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)

SRC = \
../src/domain.cpp \
../src/actor_subdomain.cpp \
../src/mpi_channel.cpp \
../src/mpi_machine.cpp \
../src/object_broker.cpp \
../src/partitioned_domain.cpp \
../src/shadow_subdomain.cpp \

INCLUDE_HEADER_LOCATION=-I../src/
LINK_LIB_LOCATION=
LINK_LIB_NAME=-lmpi

compile:
	${CC} -o test.out ${CFLAGS} main.cpp ${SRC} ${INCLUDE_HEADER_LOCATION} ${LINK_LIB_LOCATION}

clean:
	rm -f test.out
