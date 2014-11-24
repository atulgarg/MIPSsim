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
        return instruction->print();
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
        cout<<"addStation"<<endl;
        if(this->isFull())
                return false;
        reservationStations.push_back(reservationStation);
        cout<<"Adding station"<<endl;
        return true;
}
vector<RSEntry*> ReservationStations::checkPendingReservationStations(int cycle){
        vector<RSEntry*> toBeExecutedInstructions;
        for(int i=0;i<reservationStations.size();i++){
                RSEntry* reservationStation = reservationStations.at(i);
                if(reservationStation!=NULL
                                && reservationStation->isBusy()
                                && reservationStation->getCycle()< cycle
                                && reservationStation->isReady()){
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
ROBEntry::ROBEntry(bool busy, Instruction* instruction, ROBState state, int destination){
        this->busy = busy;
        this->instruction = instruction;
        this->state = state;
        this->destination = destination;
        this->value = -1;
        this->cycle = -1;
}
int ROBEntry::getCycle(){
        return this->cycle;
}
string ROBEntry::print(){
        return this->instruction->print();
}
void ROBEntry::update(int value,int cycle,ROBState robState){
        this->value = value;
        this->cycle = cycle;
        this->state = robState;
}
int ROBEntry::getValue(){
        return this->value;
}
ROBState ROBEntry::getState(){
        return this->state;
}
Instruction* ROBEntry::getInstruction(){
        return this->instruction;
}
int ROBEntry::getDestination(){
        return this->destination;
}
ROB::ROB(int max_entries){
        rob = new ROBEntry*[max_entries];
        this->max_entries = max_entries;
        this->front = -1;
        this->rear = -1;
}
vector<string> ROB::print(){
        vector<string> robEntries;
        for(int i=front;i<rear;i=(i+1)%max_entries)
                robEntries.push_back(rob[i]->getInstruction()->print());
        return robEntries;
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
                assert(true);
                //TODO check
                //cout<<"ROB full. Cannot add something is wrong should not reach here."<<endl;
                exit(0);
        }
        return rear;
}
ROBEntry* ROB::peek(){
        return rob[front];
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
void ROB::update(int robID, int value, int cycle, ROBState robstate){
        rob[robID]->update(value, cycle, robstate);
}
ROBState ROB::state(int robID){
        return rob[robID]->getState();
}
int ROB::getHeadID(){
        return front;
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
