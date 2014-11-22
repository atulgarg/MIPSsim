#include "Simulator.h"
Simulator::Simulator(map<int,Abstract*>* memory_map,int program_counter, 
                int numReservationStations, int numROBEntry, int numberOfRegisters){
        this->memory_map = memory_map;
        this->program_counter = program_counter;
        pipeline = new Pipeline(numReservationStations, numROBEntry, numberOfRegisters);
}
void Simulator::simulate(){
}
void Simulator::simulate(int m, int n){
}
