#include "PipelineUnits.h"
using namespace std;
RSEntry::RSEntry(Instruction* instruction, bool busy, int Vj, int Vk, int ROBId_Qj, int ROBId_Qk, int A, int cycle){
        this->instruction = instruction;
        this->busy = busy;
        this->Vj = Vj;
        this->Vk = Vk;
        this->ROBId_Qj = ROBId_Qj;
        this->ROBId_Qk = ROBId_Qk;
        this->A  = A;
        this->cycle = cycle;
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
int RSEntry::getCycle(){
        return this->cycle;
}

ReservationStations::ReservationStations(int max_stations){
        reservationStations.resize(max_stations);
        this->currently_used = 0;
        this->max_stations = max_stations;
}

bool ReservationStations::isFull(){
        return (currently_used < max_stations);
}

bool ReservationStations::addStation(RSEntry* reservationStation){
        if(this->isFull())
                return false;
        reservationStations.push_back(reservationStation);
        return true;
}
ROBEntry::ROBEntry(bool busy, Instruction* instruction, ROBState state, int destination){
        this->busy = busy;
        this->instruction = instruction;
        this->state = state;
        this->destination = destination;
        this->value = -1;
        this->cycle = -1;
}
void ROBEntry::update(double value, int cycle){
        this->value = value;
        this->cycle = cycle;
}
int ROBEntry::getValue(){
        return this->value;
}
ROBState ROBEntry::getState(){
        return this->state;
}
ROB::ROB(int max_entries){
        rob = new ROBEntry*[max_entries];
        this->max_entries = max_entries;
        this->front = -1;
        this->rear = -1;
}
bool ROB::isFull(){
        return ((this->front==0 && rear == max_entries-1) || front == rear+1);
}
int ROB::push(ROBEntry* robEntry){
        if(!isFull()){
                if(front == rear == -1){
                        //empty ROB
                        front++;
                        rear++;
                }else if(rear==max_entries-1)
                        rear =0;
                else
                        rear++;
                rob[rear] = robEntry;
        }else{
                //TODO check
                //cout<<"ROB full. Cannot add something is wrong should not reach here."<<endl;
                exit(0);
        }
        return rear;
}
ROBEntry* ROB::pop(){
        if(!isEmpty()){
                ROBEntry* top = rob[front];
                front++;
                return top;
        }
        return NULL;
}
bool ROB::isEmpty(){
        //TODO need to check empty condition.
        if(front == rear == -1)
                return true;
        return false;
}
void ROB::flushAfter(int robID){
       //Set Null all the ID's after the given ID. 
}
int ROB::value(int robID){
        return rob[robID]->getValue();
}
ROBState ROB::state(int robID){
        return rob[robID]->getState();
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
