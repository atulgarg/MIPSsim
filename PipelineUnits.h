#include "Instructions.h"
//Constants used for reorder Buffer
#define ROB_COMMIT 0
#define ROB_WRITE_RESULT 1
#define ROB_EXECUTE 2
enum ROBState {ROB_COMMIT, ROB_WRITE_RESULT, ROB_EXECUTE};

//Constants used for BTB.
#define PREDICTION_TAKEN 1
#define PREDICTION_NOT_TAKEN 0
enum Prediction {PREDICTION_TAKEN, PREDICTION_NOT_TAKEN};

class RSEntry{
        //Opcode
        bool busy;
        int Vj;
        int Vk;
        ReservationStation* Qj;
        ReservationStation* Qk;
        int A;
        //        dest; ?? need to verify if this can be register or only address?
        bool isBusy();
};
class ROBEntry{
        bool busy;
        Instructions* instruction;
        ROBState state;
        int destination;    //differentiate between memory and register??
        double value;
};
class RegisterStat{
        //  status;
        //  reorder;
};
class BTBEntry{
        int instructionAddress;
        int branchTargetAddress;
        Prediction prediction;
        //LRU replacement policy;
};
class RegisterFile{
};
