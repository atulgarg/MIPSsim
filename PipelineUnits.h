class ReservationStation{
        //Opcode
        bool busy;
        int Vj;
        int Vk;
        ReservationStation* Qj;
        ReservationStation* Qk;
        int A;
        //        dest;
        bool isBusy();
};
class ReorderBuffer{
        //      entry
        //      status;
        //      instruction;
        //      state;
        //      destination;
        //      value;
        //
};
class RegisterStat{
        //  status;
        //  reorder;
};
class BranchTargetBuffer{
        //instruction address;
        //branch target address;
        //1-bit predictor;
        //LRU replacement policy;
};
