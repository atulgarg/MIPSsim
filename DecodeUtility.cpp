#include "DecodeUtility.h"

DecodeUtility::DecodeUtility(RegisterStat* registerStat, RegisterFile* registerFile, ROB* rob){
        this->registerStat = registerStat;
        this->registerFile = registerFile;
        this->rob = rob;
}
void DecodeUtility::decodeUtility(int reg, int& Vj,int& Qj){
        if(registerStat->registerBusy(reg)){
                int h = registerStat->getRegisterReorderEntryID(reg);
                if(rob->state(h) == ROB_WRITE_RESULT){
                        Vj = rob->value(h);
                        Qj = -1;
                }else{
                        Qj = h;
                }
        }else{
                Vj = registerFile->getRegisterValue(reg);
                Qj = -1;
        }
}
RSEntry* DecodeUtility::decodeRTypeInstruction(R_Instruction* instruction, int cycle){
        int Qj,Qk,Vj,Vk;
        decodeUtility(instruction->rs,Vj,Qj);
        decodeUtility(instruction->rt,Vk,Qk);
        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Vk, Qj, Qk, -1, cycle,1);
        return reservationStation; 
}
RSEntry* DecodeUtility::decodeITypeInstruction(I_Instruction* instruction, int cycle){
        int Qj,Qk,Vj,Vk;
        decodeUtility(instruction->rs, Vj, Qj);
        //Qk Vk not required.
        Qk = -1; Vk = 0;
        int numCycles = 1;
        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Vk, Qj, Qk, -1, cycle, numCycles);
        return reservationStation; 
}
RSEntry* DecodeUtility::decodeLoadStoreInstruction(I_Instruction* instruction, int cycle){
        int Qj,Qk,Vj,Vk;
        decodeUtility(instruction->rs, Vj, Qj);
        int A = instruction->getImmediate();
        //Qk Vk not required.
        Qk = -1; Vk = 0;
        int numCycles = 2;
        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Vk, Qj, Qk, A, cycle, numCycles);
        return reservationStation; 
}
RSEntry* DecodeUtility::decodeJTypeInstruction(J_Instruction* instruction, int cycle){

}
RSEntry* DecodeUtility::decodeBreakTypeInstruction(BREAK* instruction, int cycle){

}
RSEntry* DecodeUtility::decodeInstruction(Instruction* instruction,int cycle){
        if(instruction->isLoad() || instruction->isStore())
                return decodeLoadStoreInstruction((I_Instruction*)instruction,cycle);
        else if(instruction->getType() == ITYPE)
                return decodeITypeInstruction((I_Instruction*)instruction,cycle);
        else if(instruction->getType() == RTYPE)
                return decodeRTypeInstruction((R_Instruction*)instruction,cycle);
        else if(instruction->getType() == JTYPE)
                return decodeJTypeInstruction((J_Instruction*)instruction, cycle);
        else if(instruction->getType() == BREAKTYPE)
                return decodeBreakTypeInstruction((BREAK*)instruction, cycle);
}
