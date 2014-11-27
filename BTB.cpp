#include "BTB.h"
BTBEntry::BTBEntry(int instructionAddress, int targetAddress, Prediction prediction){
        this->instructionAddress = instructionAddress;
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
void BTB::update(int instructionAddress, int targetAddress, Prediction prediction){
        assert(btbList.size()== btbMap.size());
        if(btbMap.find(instructionAddress)!=btbMap.end())
                btbList.erase(btbMap.find(instructionAddress)->second);
        else if(btbList.size() == max_entries){
                debug("LRU replacement for %d",(btbList.back()).instructionAddress);
                btbMap.erase(btbList.back().instructionAddress);
                btbList.pop_back();
        }
        btbList.push_front(BTBEntry(instructionAddress, targetAddress,prediction));
        btbMap[instructionAddress] = btbList.begin();
        debug("Front : %d",btbList.front().instructionAddress);
        debug("End : %d", btbList.back().instructionAddress);
}
vector<string> BTB::print(){
        vector<string> output;
        list<BTBEntry>::iterator listIter = btbList.begin();
        int entry = btbList.size();
        for(;listIter!=btbList.end();++listIter,--entry){
                BTBEntry btbEntry = *(listIter);
                stringstream ss;
                ss<<"[Entry "<<entry<<"]:<"<<btbEntry.instructionAddress<<","<<btbEntry.predictedAddress<<","<<btbEntry.prediction<<">";
                output.push_back(ss.str());
        }
        reverse(output.begin(),output.end()); 
        return output;
}
int BTB::getNumberOfEntries(){
        assert(btbMap.size() == btbList.size());
        return btbMap.size();
}
