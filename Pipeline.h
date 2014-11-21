#ifndef _PIPELINE_H_
#define _PIPELINE_H_
#include "PipelineUnits.h"
#include "Instructions.h"
#include<queue>
#include<vector>
#include<map>
using namespace std;
/**
 * InstructionQueueComparison for returning instruction with smallest cycle.
 */
class InstructionQueueComparison{
        public:
                bool operator()(const pair<int, Abstract*>& lhs, const pair<int, Abstract*>& rhs)const{
                        return (lhs.first > rhs.first);
                }
};
class Pipeline{
        private:
                map<int, Abstract*> memory_map;
                int PC; 
                priority_queue<pair<int,Abstract*>,vector<pair<int,Abstract*> >, InstructionQueueComparison> instruction_queue;
                ReservationStations reservationStations;
                ROB rob;
                /*
                int ALU_Units;
                //waiting instructions for execution.
                map<int, vector<Instruction*> > waitingInstructions;
                //instructions currently executing.
                vector<Instruction *> currentlyExecutingInstructions;
               */ 
        public:
                Pipeline(int numReservationStations, int numROBEntry);
                void instructionFetch(int cycle);
                void decodeAndIssue(int cycle);
                void execute(int cycle);
                void writeResult(int cycle);
                void commit(int cycle);
};
#endif
