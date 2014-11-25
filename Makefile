CC=g++

CFLAGS=-c -g

all: MIPSsim

MIPSsim: MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o Simulator.o DecodeUtility.o BTB.o ROB.o ReservationStation.o
	$(CC) MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o Simulator.o DecodeUtility.o BTB.o ROB.o ReservationStation.o -o MIPSsim

ROBTest: ROBTest.o ROB.o
	$(CC) ROBTest.o ROB.o -o ROBTest

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

ROB.o: ROB.cpp
	$(CC) $(CFLAGS) ROB.cpp

DecodeUtility.o: DecodeUtility.cpp
	$(CC) $(CFLAGS) DecodeUtility.cpp

BTB.o: BTB.cpp
	$(CC) $(CFLAGS) BTB.cpp

ROBTest.o: ROBTest.cpp
	$(CC) $(CFLAGS) ROBTest.cpp

ReservationStation.o: ReservationStation.cpp
	$(CC) $(CFLAGS) ReservationStation.cpp
clean:
	rm -rf *.o MIPSsim ROBTest
