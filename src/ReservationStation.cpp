#include "ReservationStation.h"
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
RSEntry::RSEntry(Instruction* instruction, bool busy, int Vj, int Vk, int ROBId_Qj, int ROBId_Qk, int cycle, int numCycles)
        :RSEntry(instruction, busy, Vj, Vk, ROBId_Qj, ROBId_Qk, -1, cycle, numCycles){
}
RSEntry::RSEntry(Instruction* instruction, bool busy, int Vj, int ROBId_Qj, int A, int cycle, int numCycles)
        :RSEntry(instruction, busy, Vj, 0, ROBId_Qj, -1, A, cycle, numCycles){
}

RSEntry::RSEntry(Instruction* instruction, bool busy, int Vj, int ROBId_Qj, int cycle, int numCycles)
        :RSEntry(instruction, busy, Vj, 0, ROBId_Qj, -1, -1, cycle, numCycles){
}

RSEntry::RSEntry(Instruction* instruction, bool busy, int cycle, int numCycles):
        RSEntry(instruction, busy, 0, 0, -1, -1, -1, cycle, numCycles){
}

string RSEntry::print(){
        return "[" + instruction->print(false) + "]";
}
bool RSEntry::isBusy(){
        return busy;
}
void RSEntry::setBusy(bool busy){
        this->busy = busy;
}
void RSEntry::updateROBId(int robID){
        this->robID = robID;
}
int RSEntry::getROBId(){
        return this->robID;
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
bool RSEntry::isStoreReady(){
        if(ROBId_Qj == -1 && instruction->isStore())
                return true;
        return false;
}
int RSEntry::getRemainingCycles(){
        return this->numCycles;
}
int RSEntry::execute(){
        return this->instruction->execute(Vj,Vk);
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
                                && (reservationStation->isReady() || reservationStation->isStoreReady())
                                && reservationStation->getRemainingCycles() > 0){
                        toBeExecutedInstructions.push_back(reservationStation);
                }
        }
        return toBeExecutedInstructions;
}
void ReservationStations::updateStations(ROB* rob){
        for(int i=0;i<reservationStations.size();i++){
                RSEntry* rs = reservationStations.at(i);
                if(rs->ROBId_Qj != -1 
                                && rob->state(rs->ROBId_Qj) == ROB_COMMIT){
                        rs->Vj = rob->value(rs->ROBId_Qj);
                        rs->ROBId_Qj = -1;
                }
                if(rs->ROBId_Qk != -1
                                && rob->state(rs->ROBId_Qk) == ROB_COMMIT){
                        rs->Vk = rob->value(rs->ROBId_Qk);
                        rs->ROBId_Qk = -1;
                }
        }
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
                                reservationStation->ROBId_Qj = -1;
                        }
                        if(reservationStation->ROBId_Qk != -1
                                        && CDB.find(reservationStation->ROBId_Qk)!=CDB.end()){
                                reservationStation->Vk = CDB.find(reservationStation->ROBId_Qk)->second;
                                reservationStation->ROBId_Qk = -1;
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
        vector<string> stations;
        for(int i=0;i<reservationStations.size();i++){
                stations.push_back(reservationStations.at(i)->print());
        }
        return stations;
}
void ReservationStations::reset(RSEntry* reservationStation){
        int index = 0;
        vector<RSEntry*> tempReservationStations;
        vector<RSEntry*>::iterator rsIter = reservationStations.begin();
        
        rsIter = reservationStations.begin();
        for(;rsIter!=reservationStations.end();++rsIter){
                if((*rsIter) == reservationStation)
                        break;
        }
        //this assertion is needed as all the stations 
        //assert(index<reservationStations.size());
        reservationStations.erase(++rsIter,reservationStations.end());
        
        currently_used = reservationStations.size(); 
}

