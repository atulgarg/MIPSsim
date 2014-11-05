#include "Instructions.h"
#define ROB_COMMIT 0
#define ROB_WRITE_RESULT 1
#define ROB_EXECUTE 2
enum ROBState {ROB_COMMIT, ROB_WRITE_RESULT, ROB_EXECUTE};
class ReservationStation{
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
class BranchTargetBufferEntry{
        int instructionAddress;
        int branchTargetAddress;
        //1-bit predictor;
        //LRU replacement policy;
};
class RegisterFile{
};
