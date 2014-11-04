#include "Instructions.h"
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
class ReorderBuffer{
        //      entry
        //      status;
        Instructions* instruction;
        //      state;
        //      destination;
        double value;
};
class RegisterStat{
        //  status;
        //  reorder;
};
class BranchTargetBuffer{
        int instructionAddress;
        int branchTargetAddress;
        //1-bit predictor;
        //LRU replacement policy;
};
