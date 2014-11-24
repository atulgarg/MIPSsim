#ifndef _PIPELINE_H_
#define _PIPELINE_H_
#include "PipelineUnits.h"
#include "Instructions.h"
#include "DecodeUtility.h"
#include "BTB.h"
#include<deque>
#include<vector>
#include<map>
#include<string>
/**
 * InstructionQueueComparison for returning instruction with smallest cycle.
 */
class InstructionQueueComparison{
        public:
                bool operator()(const pair<int, Instruction*>& lhs, const pair<int, Instruction*>& rhs)const{
                        return (lhs.first > rhs.first);
                }
};
class ALUUnit{
        public:
        void execute(RSEntry* reservationStation);
};

class Pipeline{
        private:
                map<int, Abstract*>* memory_map;
                int PC;
                int NPC; 
                deque<pair<int,Instruction*> > instruction_queue;
                BTB btb;
                ReservationStations reservationStations;
                map<int, int> CDB;
                ROB rob;
                RegisterStat registerStat;
                RegisterFile registerFile;
                vector<pair<RSEntry*, int> >executedInstruction;
                map<Instruction*, int > predictedAddress;         //predicted
                DecodeUtility decodeUtility;
                ALUUnit aluUnit;
        public:
                Pipeline(int numReservationStations, int numROBEntry, int numberOfRegister, 
                                int numBTBEntries,map<int,Abstract*>* memory_map);
                void flushCDB();
                void instructionFetch(int cycle);
                void decodeAndIssue(int cycle);
                void execute(int cycle);
                void writeResult(int cycle);
                bool commit(int cycle);
                vector<string> printPipeline();
};
#endif
