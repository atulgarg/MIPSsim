#ifndef _BTB_H_
#define _BTB_H_
#include<map>
#include<sstream>
#include<string>
#include<vector>
#include<list>
#include<assert.h>
#include<algorithm>
#include "dbg.h"
using namespace std;
//Constants used for BTB.
enum Prediction {PREDICTION_TAKEN=1, PREDICTION_NOT_TAKEN=0};
class BTBEntry{
        public:
        BTBEntry(int instructionAddress,int predictedAddress, Prediction prediction);
        int instructionAddress;
        int predictedAddress;
        Prediction prediction;
};
class BTB{
        list<BTBEntry> btbList;
        map<int, list<BTBEntry>::iterator> btbMap;
        int max_entries;
        public:
        BTB(int max_entries);
        int predict(int instructionAddress);
        void update(int instructionAddress, int targetAddress,Prediction prediction);
        vector<string> print();
        int getNumberOfEntries();
};
#endif
