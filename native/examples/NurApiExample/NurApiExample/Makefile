CC = g++
RM = rm -f

SRC = $(wildcard *.cpp)

INCLUDE = -I../../../include
LIBDEF = -L ../../../linux -lNurApix64 -lm -lpthread
CFLAGS = -g -Os

OUTPUT = nurexample
all:
	$(CC) $(INCLUDE) $(CFLAGS) $(SRC) -o $(OUTPUT) $(LIBDEF)

clean:
	$(RM) $(OUTPUT)

run: all
	LD_LIBRARY_PATH=../../../linux ./nurexample

sudorun: all
	sudo LD_LIBRARY_PATH=../../../linux ./nurexample
