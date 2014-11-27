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
        int cycle = 1;
        bool nextFetch = true;

        while(nextFetch){
                stringstream ss;
                ss<<"Cycle <"<<cycle<<">:";
                output.push_back(ss.str());

                pipeline->instructionFetch(cycle);
                pipeline->decodeAndIssue(cycle);

                vector<string> cycleOutput = pipeline->printPipeline();
                pipeline->execute(cycle);
                pipeline->writeResult(cycle);
                nextFetch = pipeline->commit(cycle);

                output.insert(output.end(),cycleOutput.begin(),cycleOutput.end());
                cycle++;
        }
        //This is only since the last three stages were not executed
        stringstream cycleHeading;
        cycleHeading<<"Final Cycle <"<<--cycle<<">:";
        output.push_back(cycleHeading.str());
        vector<string> cycleOutput = pipeline->printPipeline();
        output.insert(output.end(),cycleOutput.begin(),cycleOutput.end());
        
        return output;
}
vector<string> Simulator::simulate(int m, int n){
}
