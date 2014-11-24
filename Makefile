CC=g++

CFLAGS=-c -g

all: MIPSsim

MIPSsim: MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o Simulator.o DecodeUtility.o BTB.o
	$(CC) MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o Simulator.o DecodeUtility.o BTB.o -o MIPSsim

MIPSsim.o: MIPSsim.cpp
	$(CC) $(CFLAGS) MIPSsim.cpp

Dissembler.o: Dissembler.cpp
	$(CC) $(CFLAGS) Dissembler.cpp

Pipeline.o: Pipeline.cpp
	$(CC) $(CFLAGS) Pipeline.cpp

Simulator.o: Simulator.cpp
	$(CC) $(CFLAGS) Simulator.cpp

PipelineUnits.o: PipelineUnits.cpp
	$(CC) $(CFLAGS) PipelineUnits.cpp

DecodeUtility.o: DecodeUtility.cpp
	$(CC) $(CFLAGS) DecodeUtility.cpp

BTB.o: BTB.cpp
	$(CC) $(CFLAGS) BTB.cpp

clean:
	rm -rf *.o MIPSsim
