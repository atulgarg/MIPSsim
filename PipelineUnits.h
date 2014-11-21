#ifndef _PIPELINE_UNITS_H_
#define _PIPELINE_UNITS_H_
#include "Instructions.h"
#include<vector>
#include<queue>
#include<cstdlib>
//Test comment.
//Constants used for reorder Buffer 
enum ROBState { ROB_COMMIT, ROB_WRITE_RESULT, ROB_EXECUTE };

//Constants used for BTB.
enum Prediction {PREDICTION_TAKEN, PREDICTION_NOT_TAKEN};

class RSEntry{
        //Opcode
        bool busy;
        int Vj;
        int Vk;
        int ROBId_Qj;
        int ROBId_Qk;
        int A;
        int reorderEntryID; //destination.
        int cycle;
        public:
        RSEntry(bool busy,int Vj,int Vk,int ROBId_Qj,int ROBId_Qk,int A,int cycle);
        bool isBusy();
        void setBusy(bool busy);
        void updateReorderID(int reorderEntryID);
        int getCycle();
};
class ReservationStations{
        vector<RSEntry*> reservationStations;
        int currently_used;
        int max_stations;
        public:
        ReservationStations(int max_stations);
        bool isFull();
        bool addStation(RSEntry* reservationStation);
};
class ROBEntry{
        bool busy;
        Instruction* instruction;
        ROBState state;
        int destination;    //differentiate between memory and register??
        double value;
        int cycle;          //for specifying cycle in which the result was put.
        public:
        public:
        ROBEntry(bool busy, Instruction* instruction, ROBState state, int destination);
        void update(double value, int cycle);
};
class ROB{
        //how to retrieve value.
        ROBEntry** rob;
        int front;
        int rear;
        int max_entries;
        public:
        ROB(int max_entries);
        int push(ROBEntry* robEntry);
        ROBEntry* pop();
        bool isEmpty();
        bool isFull();
        void flushAfter(int robID);
};
class RegisterStatEntry{
        bool busy;
        int reorderEntryID;                 //id for reorder entry
        public:
        RegisterStatEntry();
        void update(bool busy, int reorderEntryID);
        bool isBusy();
};
class RegisterStat{
        RegisterStatEntry* registerStat;
        int numberOfRegisters;
        public:
        RegisterStat(int numberOfRegisters);
        void updateRegister(int registerID, bool busy, int reorderID);
        bool registerBusy(int registerID);
};
class BTBEntry{
        int instructionAddress;
        int branchTargetAddress;
        Prediction prediction;
        //LRU replacement policy;
};
class RegisterFile{
        int* registerValue;
        int numberOfRegisters; 
        public:
        RegisterFile(int numberOfRegisters);
        void setRegisterValue(int registerID, int value);
        int getRegisterValue(int registerID);
};
#endif
