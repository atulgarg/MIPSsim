#include "PipelineUnits.h"
using namespace std;
RSEntry::RSEntry(Instruction* instruction, bool busy, int Vj, int Vk, int ROBId_Qj, int ROBId_Qk, int A, int cycle, int numCycles){
        this->instruction = instruction;
        this->busy = busy;
        this->Vj = Vj;
        this->Vk = Vk;
        this->ROBId_Qj = ROBId_Qj;
        this->ROBId_Qk = ROBId_Qk;
        this->A  = A;
        this->cycle = cycle;
        this->numCycles = numCycles;
}
string RSEntry::print(){
        return instruction->print(false);
}
bool RSEntry::isBusy(){
        return busy;
}
void RSEntry::setBusy(bool busy){
        this->busy = busy;
}
void RSEntry::updateReorderID(int robID){
        this->A = robID;
}
int RSEntry::getReorderID(){
        return this->A;
}
int RSEntry::getCycle(){
        return this->cycle;
}
bool RSEntry::isReady(){
        if(ROBId_Qj == -1 && ROBId_Qk == -1)
                return true;
        else
                return false;
}
int RSEntry::getDestination(){
        return this->destination;
}
int RSEntry::getRemainingCycles(){
        return this->numCycles;
}
int RSEntry::execute(){
        //TODO
        this->numCycles--;
        //return this->instruction->execute(Vj,Vk);
}
ReservationStations::ReservationStations(int max_stations){
        this->currently_used = 0;
        this->max_stations = max_stations;
}

bool ReservationStations::isFull(){
        return (currently_used >= max_stations);
}

bool ReservationStations::addStation(RSEntry* reservationStation){
        if(this->isFull())
                return false;
        reservationStations.push_back(reservationStation);
        return true;
}
vector<RSEntry*> ReservationStations::checkPendingReservationStations(int cycle){
        vector<RSEntry*> toBeExecutedInstructions;
        for(int i=0;i<reservationStations.size();i++){
                RSEntry* reservationStation = reservationStations.at(i);
                if(reservationStation!=NULL
                                && reservationStation->isBusy()
                                && reservationStation->getCycle()< cycle
                                && reservationStation->isReady()
                                && reservationStation->getRemainingCycles()>0){
                toBeExecutedInstructions.push_back(reservationStation);
                }
        }
        return toBeExecutedInstructions;
}
void ReservationStations::updateStations(map<int,int> CDB){
        for(int i=0;i<reservationStations.size();i++){
                RSEntry* reservationStation = reservationStations.at(i);
                if(reservationStation != NULL
                                && reservationStation->isBusy()
                                && !reservationStation->isReady()){
                        if(reservationStation->ROBId_Qj != -1            //reservationStation not ready
                                        && CDB.find(reservationStation->ROBId_Qj)!=CDB.end()){       //and value is in CDB
                                reservationStation->Vj = CDB.find(reservationStation->ROBId_Qj)->second;
                        }
                        if(reservationStation->ROBId_Qk != -1
                                        && CDB.find(reservationStation->ROBId_Qk)!=CDB.end()){
                                reservationStation->Vk = CDB.find(reservationStation->ROBId_Qk)->second;
                        }
                }
        }
}
void ReservationStations::remove(RSEntry* reservationStation){
        int index = 0;
        for(;index<reservationStations.size();index++){
                if(reservationStations.at(index) == reservationStation)
                        break;
        }
        assert(index<reservationStations.size());
        reservationStations.erase(reservationStations.begin()+ index);
        currently_used--; 
}
vector<string> ReservationStations::print(){
        cout<<"RS size:: "<<reservationStations.size()<<endl;
        vector<string> stations;
        for(int i=0;i<reservationStations.size();i++){
                stations.push_back(reservationStations.at(i)->print());
        }
        return stations;
}
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
        return registerStat[registerID].isBusy();
}
int RegisterStat::getRegisterReorderEntryID(int registerID){
        return registerStat[registerID].getReorderEntryID();
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
