#include "PipelineUnits.h"
using namespace std;
RegisterStatEntry::RegisterStatEntry(){
        this->busy = false;
        this->reorderEntryID = -1;
}
//Register Status Entry for register file.
void RegisterStatEntry::update(bool busy, int reorderEntryID){
        this->busy = busy;
        this->reorderEntryID = reorderEntryID;
}
bool RegisterStatEntry::isBusy(){
        return this->busy;
}
int RegisterStatEntry::getReorderEntryID(){
        return this->reorderEntryID;
}
RegisterStat::RegisterStat(int numberOfRegisters){
        this->numberOfRegisters = numberOfRegisters;
        registerStat = new RegisterStatEntry[numberOfRegisters];
}
void RegisterStat::updateRegister(int registerID, bool status, int reorderEntryID){
        registerStat[registerID].update(status,reorderEntryID);
}
bool RegisterStat::registerBusy(int registerID){
        if(registerID < numberOfRegisters)
                return registerStat[registerID].isBusy();
        return false;
}
int RegisterStat::getRegisterReorderEntryID(int registerID){
        return registerStat[registerID].getReorderEntryID();
}
void RegisterStat::reset(int registerID){
        registerStat[registerID].update(false,-1);
}
//Register File Operations.
RegisterFile::RegisterFile(int numberOfRegisters){
        this->numberOfRegisters = numberOfRegisters;
        this->registerValue = new int[numberOfRegisters];
        for(int i=0;i<numberOfRegisters;i++)
                this->registerValue[i] = 0;
}
int RegisterFile::getRegisterValue(int registerID){
        return this->registerValue[registerID];
}
void RegisterFile::setRegisterValue(int registerID, int value){
        this->registerValue[registerID] = value;
}
vector<string> RegisterFile::print(){
        vector<string> registerValues;
        int regIndex = 0;
        for(int i=0;i<numberOfRegisters;i+=8){
                stringstream ss;
                if(i<16)
                        ss<<"R0"<<i<<": ";
                else
                        ss<<"R"<<i<<": ";
                do{
                        ss<<getRegisterValue(regIndex)<<" ";
                        regIndex++;
                }while(regIndex%8 !=0);
                registerValues.push_back(ss.str());
        }
        return registerValues;
}
