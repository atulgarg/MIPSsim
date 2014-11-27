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
        
        while(cycle < 47){
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
                cout<<"Cycle ended"<<endl;
                cycle++;
        }
        cout<<memory_map->find(664)->second->print(false)<<endl;
        return output;
}
vector<string> Simulator::simulate(int m, int n){
}
