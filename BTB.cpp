#include "BTB.h"
BTBEntry::BTBEntry(int targetAddress, Prediction prediction){
        this->predictedAddress = targetAddress;
        this->prediction = prediction;
}
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
        if(btbMap.find(instructionAddress)!=btbMap.end())
                btbList.erase(btbMap.find(instructionAddress)->second);
        else if(btbList.size()>= max_entries){
                btbList.pop_back();
        }

        btbList.push_front(BTBEntry(targetAddress,PREDICTION_TAKEN));
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

