#ifndef _BTB_H_
#define _BTB_H_
#include<map>
#include<sstream>
#include<string>
#include<vector>
#include<list>
using namespace std;
//Constants used for BTB.
enum Prediction {PREDICTION_TAKEN, PREDICTION_NOT_TAKEN};
class BTBEntry{
        public:
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
        void update(int instructionAddress, int targetAddress);
        vector<string> print();
};
#endif
