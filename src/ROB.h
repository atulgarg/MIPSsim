#ifndef _ROB_H_
#define _ROB_H_
#include "Instructions.h"
#include "PipelineUnits.h"
#include "dbg.h"
#include<cstdlib>
#include<list>
#include<assert.h>
#include<vector>
using namespace std;
//Constants used for reorder Buffer 
enum ROBState { ROB_COMMIT, ROB_WRITE_RESULT, ROB_EXECUTE };

class ROBEntry{
        friend class ROB;
        bool busy;
        Instruction* instruction;
        ROBState state;
        int destination;    //differentiate between memory and register??
        int value;
        int cycle;          //for specifying cycle in which the result was put.
        public:
        ROBEntry(bool busy, Instruction* instruction, ROBState state, int destination);
        ROBEntry(bool busy, Instruction* instruction, ROBState state);
        void update(int value, int cycle, ROBState state);
        void update(int value, int cycle, ROBState state, int destination);
        int getValue();
        ROBState getState();
        Instruction* getInstruction();
        int getDestination();
        int getCycle();
        std::string print();
};
class ROB{
        list<ROBEntry*> rob;
        map<int, ROBEntry*> robMap; 
        int count;
        int max_entries;
        list<ROBEntry*>::iterator find(int robID);
        public:
        ROB(int max_entries);
        int push(ROBEntry* robEntry);
        ROBEntry* peek();
        ROBEntry* pop();
        bool isEmpty();
        bool isFull();
        void reset(int robID, RegisterStat* registerStat);
        int value(int robID);
        ROBState state(int robID);
        void update(int robID, int value, int cycle, ROBState state);
        void update(int robID, int value, int cycle, ROBState state, int destination);
        void setDestination(int destination,int robID);
        bool allAddressComputedBefore(int robID);
        bool noPendingStore(int robID);
        int getHeadID();
        int size();
        std::vector<std::string> print();
};
#endif
