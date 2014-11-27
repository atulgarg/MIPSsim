#include "ROB.h"
ROBEntry::ROBEntry(bool busy, Instruction* instruction, ROBState state, int destination){
        this->busy = busy;
        this->instruction = instruction;
        this->state = state;
        this->destination = destination;
        this->value = -1;
        this->cycle = -1;
}
ROBEntry::ROBEntry(bool busy, Instruction* instruction, ROBState state)
        :ROBEntry(busy,instruction,state,-1){
}

int ROBEntry::getCycle(){
        return this->cycle;
}
string ROBEntry::print(){
        return "[" +this->instruction->print(false) + "]";
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
        count = 0;
        this->max_entries = max_entries;
}
vector<string> ROB::print(){
        vector<string> robEntries;
        if(!isEmpty()){
                assert(rob.size()== robMap.size());
                map<int,ROBEntry*>::iterator iter = robMap.begin();
                for(;iter!=robMap.end();++iter){
                        robEntries.push_back("["+iter->second->getInstruction()->print(false)+ "]");
                }
        }
        return robEntries;
}
bool ROB::isFull(){
        assert(rob.size()== robMap.size());
        return (rob.size() == max_entries); 
}
int ROB::push(ROBEntry* robEntry){
        assert(rob.size()== robMap.size());
        rob.push_front(robEntry);
        robMap[count++] = robEntry;
        assert(rob.size()== robMap.size());
        return (count-1);
}
ROBEntry* ROB::peek(){
        return rob.back();
}
ROBEntry* ROB::pop(){
        if(!isEmpty()){
                ROBEntry* top = rob.back();
                map<int,ROBEntry*>::iterator iter = robMap.begin();
                for(;iter!=robMap.end();++iter){
                        if(iter->second == top)
                                break;
                }
                rob.pop_back();
                robMap.erase(iter);
                return top;
        }
        return NULL;
}
bool ROB::isEmpty(){
        return (rob.size() == 0);
}
void ROB::reset(int robID){
        //CHECK
        map<int, ROBEntry*> newMap;
        list<ROBEntry*> newList;
        ROBEntry* entry = robMap.find(robID)->second;
        map<int,ROBEntry*>::iterator iter = robMap.begin();
        for(;iter!=robMap.end();++iter){
                if(iter->first <= robID){
                        newMap.insert(*iter);
                        newList.push_front(iter->second);
                }
        }
        rob = newList;
        robMap = newMap;
        assert(rob.size() == robMap.size());
}
int ROB::value(int robID){
        ROBEntry* entry = robMap.find(robID)->second;
        return entry->getValue();
}
void ROB::update(int robID, int value, int cycle, ROBState robstate){
        ROBEntry* entry = robMap.find(robID)->second;
        entry->update(value, cycle, robstate);
}
void ROB::update(int robID, int value, int cycle, ROBState robstate, int destination){
        ROBEntry* entry = robMap.find(robID)->second;
        entry->update(value, cycle, robstate, destination);
}

ROBState ROB::state(int robID){
        ROBEntry* entry = robMap.find(robID)->second;
        return (entry)->getState();
}
int ROB::getHeadID(){
        ROBEntry* top = rob.back();
        map<int,ROBEntry*>::iterator iter = robMap.begin();
        for(;iter!=robMap.end();++iter){
                if(iter->second == top)
                        break;
        }
        return iter->first;
}
bool ROB::allAddressComputedBefore(int robID){
        if(!isEmpty()){
                map<int,ROBEntry*>::iterator iter = robMap.begin();
                for(;iter!=robMap.end();++iter){
                        if(iter->first < robID){
                                ROBEntry* robEntry = iter->second;
                                Instruction* instruction = robEntry->getInstruction();
                                if((instruction->isLoad() || instruction->isStore()) 
                                                && robEntry->destination == -1)
                                        return false;
                        }

                }
        }
        return true;
}
bool ROB::noPendingStore(int robID){
        if(!isEmpty()){
                ROBEntry* entry = robMap.find(robID)->second;
                map<int,ROBEntry*>::iterator iter = robMap.begin();
                for(;iter!=robMap.end();++iter){
                        ROBEntry* robEntry = iter->second;
                        if(iter->first < robID){
                                Instruction* instruction = robEntry->getInstruction();
                                if((instruction->isLoad() || instruction->isStore()) 
                                                && robEntry->destination == entry->destination
                                                && robEntry->state != ROB_COMMIT)
                                        return false;
                        }
                }
        }
        return true;
}
int ROB::size(){
        return rob.size();
}
void ROB::setDestination(int destination, int robID){
        ROBEntry* entry = robMap.find(robID)->second;
        (entry)->destination = destination;
}
