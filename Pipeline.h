#include "PipelineUnits.h"
#include<queue>
using namespace std;
class Pipeline{
        private:
                map<int, Abstract<Instruction*> > memory_map;
                int PC; 
                queue<Abstract*> instruction_queue;
                //10 integer ALU RS entries
                ReservationStations reservationStations;
                //6 Entries
                ROB rob;
        public:
                void instructionFetch();
                void decodeAndIssue();
                void execute();
                void writeResult();
                void commit();
};
