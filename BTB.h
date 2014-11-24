#ifndef _BTB_H_
#define _BTB_H_
#include<map>

using namespace std;
//Constants used for BTB.
enum Prediction {PREDICTION_TAKEN, PREDICTION_NOT_TAKEN};
class BTB{
        map<int, pair<int,Prediction> > btb;
        int max_entries;
        public:
        BTB(int max_entries);
        int predict(int instructionAddress);
        void update();
        
};
#endif
