CC=g++

CFLAGS=-c -g -std=c++11

all: MIPSsim

MIPSsim: MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o Simulator.o DecodeUtility.o BTB.o ROB.o ReservationStation.o
	$(CC) MIPSsim.o Dissembler.o Pipeline.o PipelineUnits.o Simulator.o DecodeUtility.o BTB.o ROB.o ReservationStation.o -o MIPSsim

ROBTest: ROBTest.o ROB.o
	$(CC) ROBTest.o ROB.o -o ROBTest

BTBTest: BTBTest.o BTB.o
	$(CC) BTBTest.o BTB.o -o BTBTest

%.o: %.cpp
	$(CC) $(CFLAGS) $^

clean:
	rm -rf *.o MIPSsim ROBTest BTBTest output.txt
