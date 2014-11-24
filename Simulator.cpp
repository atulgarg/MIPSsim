#include "Simulator.h"
Simulator::Simulator(map<int,Abstract*>* memory_map,int program_counter, 
                int numReservationStations, int numROBEntry, int numberOfRegisters,
                int numBTBEntries){
        this->memory_map = memory_map;
        this->program_counter = program_counter;
        pipeline = new Pipeline(numReservationStations, numROBEntry, numberOfRegisters, numBTBEntries,memory_map);
}
vector<string> Simulator::simulate(){
        vector<string> output;
        int cycle = 0;
        bool nextFetch = true;
        while(cycle < 2){
                pipeline->instructionFetch(cycle);
                pipeline->decodeAndIssue(cycle);

                pipeline->execute(cycle);
                pipeline->writeResult(cycle);
                nextFetch = pipeline->commit(cycle);
                cycle++;
                output = pipeline->printPipeline();
                cout<<"Cycle ended"<<endl;
        }
        return output;
}
vector<string> Simulator::simulate(int m, int n){
}
