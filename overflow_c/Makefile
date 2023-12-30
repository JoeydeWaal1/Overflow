CXX =  gcc
LD  =  gcc

CCFLAGS  = -std=c2x
CCFLAGS += -Wall
CCFLAGS += -o3


all: clean main

main:
	$(LD) -o main main.c

run: all
	./main

clean:
	rm -f ./main
