#include "PipelineUnits.h"
RSEntry::RSEntry(bool busy,int Vj,int Vk,RSEntry* Qj,RSEntry* Qk,int A){
        this->busy = busy;
        this->Vj = Vj;
        this->Vk = Vk;
        this->Qj = Qj;
        this->Qk = Qk;
        this->A  = A;
}
bool RSEntry::isBusy(){
        return busy;
}
void RSEntry::setBusy(bool busy){
        this->busy = busy;
}
ReservationStations::ReservationStations(int max_stations){
        this->currently_used = 0;
        this->max_stations = max_stations;
}
int ReservationStations::findNextEmpty(){
        for(int i=0;i<10;i++){
                if(reservationStations[i] == NULL)
                        return i;
        }
}
/**
 * Get a reservationStation if there is one empty.
 */
RSEntry* ReservationStations::getReservationStation(){
        if(currently_used < max_stations){
                this->currently_used++;
                return reservationStations[findNextEmpty()]; 
        }
}
ROB::ROB(int max_entries){
        rob = new *ROBEntry[max_entries]();
        this->max_entries = max_entries;
        this->front = -1;
        this->rear = -1;
}
bool ROB::isFull(){
        return ((this->front==0 && rear = max_entries-1) || front == rear+1);
}
void ROB::addROBEntry(ROBEntry* robEntry){
        if(!isFull()){
                if(front == rear == -1){
                        //empty ROB
                        front++;
                        rear++;
                }else if(rear==max_entries-1)
                        rear =0;
                else
                        rear++;
                ROB[rear] = robEntry;
        }else
                cout<<"ROB full. Cannot add"<<endl;
}
//Register Status Entry for register file.
RegisterStatEntry::RegisterStatEntry(){
        this->busy = false;
        this->reorderEntryID = -1;
}
bool RegisterStatEntry::isBusy(){
        return isBusy;
}
void RegisterStatEntry::update(bool busy, int reorderEntryID){
        this->busy = busy;
        this->reorderEntryID = reorderEntryID;
}
//Register File Operations.
RegisterFile::RegisterFile(int num_registers){
        this->num_registers = num_registers;
        registers = new RegisterStatEntry[num_registers];
}
bool RegisterFile::isRegisterBusy(int reg){
        return registers[reg]->isBusy();
}
void RegisterFile::updateRegister(int reg,bool busy, int reorderEntryID){
        registers[reg]->update;
}
