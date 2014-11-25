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
class RegisterFile{
        int* registerValue;
        int numberOfRegisters; 
        public:
        RegisterFile(int numberOfRegisters);
        void setRegisterValue(int registerID, int value);
        int getRegisterValue(int registerID);
        vector<string> print();
};
#endif
