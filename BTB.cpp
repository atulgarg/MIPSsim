#include "BTB.h"

BTB::BTB(int max_entries){
        this->max_entries = max_entries;
}
int BTB::predict(int instructionAddress){
        map<int, pair<int, Prediction> >::iterator iter = btb.find(instructionAddress);
        int predictedAddress;
        if(iter != btb.end() && iter->second.second == PREDICTION_TAKEN)
                predictedAddress = iter->second.first;
        else
                predictedAddress = instructionAddress + 4;
        return predictedAddress;
}


