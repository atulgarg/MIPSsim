#include "BTB.h"
BTB::BTB(int max_entries){
        this->max_entries = max_entries;
}
int BTB::predict(int instructionAddress){
        map<int, list<BTBEntry>::iterator>::iterator mapIter = btbMap.find(instructionAddress);
        int predictedAddress;
        if(mapIter != btbMap.end()){
                BTBEntry btbEntry = *(mapIter->second);
                if(btbEntry.prediction == PREDICTION_TAKEN)
                        return btbEntry.predictedAddress;
        }
        return instructionAddress + 4;
}
void BTB::update(int instructionAddress, int targetAddress){
        if(btbMap.size() < max_entries){
                map<int, list<BTBEntry>::iterator>::iterator iter = btbMap.find(instructionAddress);
                //there exists old entry for btb.
                if(iter!=btbMap.end()){
                }
        }
}
vector<string> BTB::print(){
        vector<string> output;
        map<int, list<BTBEntry>::iterator>::iterator mapIter = btbMap.begin();
        int entry = 1;
        for(;mapIter!=btbMap.end();++mapIter,++entry){
                BTBEntry btbEntry = *(mapIter->second);
                stringstream ss;
                ss<<"[Entry "<<entry<<"]:<"<<mapIter->first<<","<<btbEntry.predictedAddress<<","<<btbEntry.prediction<<">";
                output.push_back(ss.str());
        } 
        return output;
}

