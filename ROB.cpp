#include "ROB.h"
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
        return this->instruction->print(false);
}
void ROBEntry::update(int value,int cycle,ROBState robState){
        this->value = value;
        this->cycle = cycle;
        this->state = robState;
}
void ROBEntry::update(int value,int cycle,ROBState robState, int destination){
        this->value = value;
        this->cycle = cycle;
        this->state = robState;
        this->destination = destination;
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
        if(!isEmpty()){
                for(int i=front;i<=rear;i=(i+1)%max_entries)
                        robEntries.push_back(rob[i]->getInstruction()->print(false));
        }
        return robEntries;
}
bool ROB::isFull(){
        return (front == ((rear+1)%max_entries));
}
int ROB::push(ROBEntry* robEntry){
        if(!isFull()){
                if(front == -1){
                        //empty ROB
                        front++;
                }
                rear = (rear + 1)%max_entries;
                rob[rear] = robEntry;
        }else{
                assert(true);
                cout<<"ROB full. Cannot add something is wrong should not reach here."<<endl;
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
                if(front == rear){
                        front = rear = -1;
                }else
                        front = (front +1)% max_entries;
                return top;
        }
        return NULL;
}
bool ROB::isEmpty(){
        if(front == rear  && (rear == -1))
                return true;
        return false;
}
void ROB::reset(int robID){
        //TODO
       //Set Null all the ID's after the given ID.
       rear = robID;
}
int ROB::value(int robID){
        return rob[robID]->getValue();
}
void ROB::update(int robID, int value, int cycle, ROBState robstate){
        rob[robID]->update(value, cycle, robstate);
}
void ROB::update(int robID, int value, int cycle, ROBState robstate, int destination){
        rob[robID]->update(value, cycle, robstate,destination);
}

ROBState ROB::state(int robID){
        return rob[robID]->getState();
}
int ROB::getHeadID(){
        return front;
}
bool ROB::allAddressComputedBefore(int robID){
        if(!isEmpty()){
                for(int i=(robID+1)%max_entries;i<rear;i=(i+1)%max_entries){
                        Instruction* instruction = rob[i]->getInstruction();
                        if((instruction->isLoad() || instruction->isStore()) 
                                        && rob[i]->destination == ((Lw*)instruction)->getImmediate())
                                return false;
                }
        }
        return true;
}
bool ROB::noPendingStore(int robID){
        if(!isEmpty()){
                for(int i=(robID+1)%max_entries;i<rear;i=(i+1)%max_entries){
                        Instruction* instruction = rob[i]->getInstruction();
                        if((instruction->isLoad() || instruction->isStore()) 
                                        && rob[i]->destination == rob[robID]->destination
                                        && rob[i]->state != ROB_COMMIT)
                                return false;
                }
        }
        return true;
}
void ROB::setDestination(int destination, int robID){
        this->rob[robID]->destination = destination;
}
