CC=g++

CFLAGS=-c

all: MIPSsim

MIPSsim: MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o
	$(CC) MIPSsim.o Dissembler.o -o MIPSsim

MIPSsim.o: MIPSsim.cpp
	$(CC) $(CFLAGS) MIPSsim.cpp

Dissembler.o: Dissembler.cpp
	$(CC) $(CFLAGS) Dissembler.cpp

Pipeline.o: Pipeline.cpp
	$(CC) $(CFLAGS) Pipeline.cpp

PipelineUnits.o: PipelineUnits.cpp
	$(CC) $(CFLAGS) PipelineUnits.cpp
clean:
	rm -rf *.o MIPSsim
