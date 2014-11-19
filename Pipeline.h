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
                int ALU_Units;
                //waiting instructions for execution.
                map<int, vector<Instruction*> > waiting_instructions;
                //instructions currently executing.
                vector<Instruction *> currentlyExecutingInstructions; 
        public:
                void instructionFetch();
                void decodeAndIssue();
                void execute();
                void writeResult();
                void commit();
};
