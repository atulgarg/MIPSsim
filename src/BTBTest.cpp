#include "BTB.h"
#include<vector>
#include<iostream>
#include<string>
using namespace std;
void printVector(vector<string> list){
        for(int i=0;i<list.size();i++)
                cout<<list.at(i);
        cout<<endl;
}
int main(){
        BTB btb(4);
        cout<<"Number of entries in empty BTB: "<<btb.getNumberOfEntries();
        cout<<"Adding max entries to btb"<<endl;
        btb.update(600,700,PREDICTION_TAKEN);
        btb.update(604,704,PREDICTION_NOT_TAKEN);
        btb.update(608,708,PREDICTION_NOT_TAKEN);
        btb.update(612,712,PREDICTION_TAKEN);
        cout<<"Printing BTB"<<endl;
        printVector(btb.print());
        cout<<"Updating 608 "<<endl;
        btb.update(608,808,PREDICTION_TAKEN);
        printVector(btb.print());
        cout<<"Now adding another entry 600 should be removed"<<endl;
        btb.update(616,716,PREDICTION_TAKEN);
        cout<<"Printing BTB"<<endl;
        printVector(btb.print());
        return 0;
}
