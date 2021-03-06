#include "ROB.h"
int main(){
        ROB rob(7);
        ROBEntry* robEntry1 = new ROBEntry(false, NULL, ROB_EXECUTE, 0);
        ROBEntry* robEntry2 = new ROBEntry(false, NULL, ROB_EXECUTE, 0);
        ROBEntry* robEntry3 = new ROBEntry(false, NULL, ROB_EXECUTE, 0);
        ROBEntry* robEntry4 = new ROBEntry(false, NULL, ROB_EXECUTE, 0);
        ROBEntry* robEntry5 = new ROBEntry(false, NULL, ROB_EXECUTE, 0);
        ROBEntry* robEntry6 = new ROBEntry(false, NULL, ROB_EXECUTE, 0);
        if(rob.isEmpty())
                cout<<"Correct rob empty"<<endl;
        cout<<"First entry inserted at:"<<rob.push(robEntry1)<<endl;
        cout<<"Next entry inserted at: "<<rob.push(robEntry2)<<endl;
        cout<<"Next entry inserted at: "<<rob.push(robEntry3)<<endl;
        cout<<"Next entry inserted at:"<<rob.push(robEntry4)<<endl;
        cout<<"Next entry inserted at:"<<rob.push(robEntry5)<<endl;
        cout<<"Next entry inserted at: "<<rob.push(robEntry6)<<endl;
        cout<<"Size "<<rob.size()<<endl;
        cout<<"Head "<<rob.getHeadID()<<endl;
        cout<<"Resetting all entries after 3"<<endl;
        rob.reset(3);
        cout<<"Size "<<rob.size()<<endl;
        cout<<"Next entry inserted at"<<rob.push(robEntry6)<<endl;
        if(rob.isFull())
                cout<<"Correct rob Full"<<endl;
        return 0;
}
