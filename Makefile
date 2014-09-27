CC=g++

CFLAGS=-c

all: MIPSsim

MIPSsim: MIPSsim.o Dissembler.o
	$(CC) MIPSsim.o Dissembler.o -o MIPSsim

MIPSsim.o: MIPSsim.cpp
	$(CC) $(CFLAGS) MIPSsim.cpp

Dissembler.o: Dissembler.cpp
	$(CC) $(CFLAGS) Dissembler.cpp

clean:
	rm -rf *.o MIPSsim
