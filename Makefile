CC=g++

CFLAGS=-c -g -std=c++11
LIB=lib
SRC=src
all: MIPSsim

MIPSsim: $(LIB)/MIPSsim.o $(LIB)/Dissembler.o $(LIB)/Pipeline.o $(LIB)/PipelineUnits.o $(LIB)/Simulator.o $(LIB)/DecodeUtility.o $(LIB)/BTB.o $(LIB)/ROB.o $(LIB)/ReservationStation.o
	$(CC) $(LIB)/MIPSsim.o $(LIB)/Dissembler.o $(LIB)/Pipeline.o $(LIB)/PipelineUnits.o $(LIB)/Simulator.o $(LIB)/DecodeUtility.o $(LIB)/BTB.o $(LIB)/ROB.o $(LIB)/ReservationStation.o -o MIPSsim

ROBTest: $(LIB)/ROBTest.o $(LIB)/ROB.o
	$(CC) $(LIB)/ROBTest.o $(LIB)/ROB.o -o ROBTest

BTBTest: $(LIB)/BTBTest.o $(LIB)/BTB.o
	$(CC) $(LIB)/BTBTest.o $(LIB)/BTB.o -o BTBTest

$(LIB)/%.o: $(SRC)/%.cpp
	@mkdir -p lib
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(LIB)/*.o MIPSsim ROBTest BTBTest output.txt
