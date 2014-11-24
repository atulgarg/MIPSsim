#include "Pipeline.h"
class Simulator{
        Pipeline* pipeline;
        map<int,Abstract*>* memory_map;
        int program_counter; 
        public:
        Simulator(map<int,Abstract*>* memory_map,int program_counter, 
                int numReservationStations, int numROBEntry, int numberOfRegisters, int numBTBEntries);
        vector<string> simulate(int m,int n);
        vector<string> simulate();
};
