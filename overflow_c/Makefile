CXX =  clang
LD  =  clang

CCFLAGS  = -std=c2x
CCFLAGS += -Wall
# CCFLAGS += -o3
#


all: main

main:
	$(LD) -o main main.c

run: all
	./main

clean:
	rm ./main
