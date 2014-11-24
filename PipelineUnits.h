#ifndef _PIPELINE_UNITS_H_
#define _PIPELINE_UNITS_H_
#include<vector>
#include<queue>
#include<cstdlib>
#include<map>
#include<string>
#include<assert.h>
#include "Instructions.h"
#include "BTB.h"
//Constants used for reorder Buffer 
enum ROBState { ROB_COMMIT, ROB_WRITE_RESULT, ROB_EXECUTE };

class RSEntry{
        friend class ALUUnit;
        friend class Pipeline;
        Instruction* instruction;
        bool busy;
        int A;
        int reorderEntryID, destination, cycle, numCycles;          //number of cycles required for execution.
        int result;
        public:
        int ROBId_Qj, ROBId_Qk, Vj, Vk;
        RSEntry(Instruction* instruction,bool busy,int Vj,int Vk,int ROBId_Qj,int ROBId_Qk,int A,int cycle, int numCycles);
        bool isBusy();
        void setBusy(bool busy);
        void updateReorderID(int reorderEntryID);
        int getReorderID();
        int getCycle();
        bool isReady();
        int getDestination();
        int execute();
        int getRemainingCycles();
        std::string print();
};
class ReservationStations{
        std::vector<RSEntry*> reservationStations;
        int currently_used;
        int max_stations;
        public:
        ReservationStations(int max_stations);
        bool isFull();
        bool addStation(RSEntry* reservationStation);
        void updateStations(std::map<int,int> CDB);
        std::vector<RSEntry*> checkPendingReservationStations(int cycle);
        void remove(RSEntry* reservationStation);
        std::vector<std::string> print();
};
class ROBEntry{
        bool busy;
        Instruction* instruction;
        ROBState state;
        int destination;    //differentiate between memory and register??
        int value;
        int cycle;          //for specifying cycle in which the result was put.
        public:
        ROBEntry(bool busy, Instruction* instruction, ROBState state, int destination);
        void update(int value, int cycle, ROBState state);
        int getValue();
        ROBState getState();
        Instruction* getInstruction();
        int getDestination();
        int getCycle();
        std::string print();
};
class ROB{
        ROBEntry** rob;
        int front;
        int rear;
        int max_entries;
        public:
        ROB(int max_entries);
        int push(ROBEntry* robEntry);
        ROBEntry* peek();
        ROBEntry* pop();
        bool isEmpty();
        bool isFull();
        void flushAfter(int robID);
        int value(int robID);
        ROBState state(int robID);
        void update(int robID, int value, int cycle, ROBState state);
        int getHeadID();
        std::vector<std::string> print();
};
class RegisterStatEntry{
        bool busy;
        int reorderEntryID;                 //id for reorder entry
        public:
        RegisterStatEntry();
        void update(bool busy, int reorderEntryID);
        int getReorderEntryID();
        bool isBusy();
};
class RegisterStat{
        RegisterStatEntry* registerStat;
        int numberOfRegisters;
        public:
        RegisterStat(int numberOfRegisters);
        void updateRegister(int registerID, bool busy, int reorderID);
        bool registerBusy(int registerID);
        int getRegisterReorderEntryID(int registerID);
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
