#include "DecodeUtility.h"

DecodeUtility::DecodeUtility(RegisterStat* registerStat, RegisterFile* registerFile, ROB* rob){
        this->registerStat = registerStat;
        this->registerFile = registerFile;
        this->rob = rob;
}
void DecodeUtility::markRegBusy(int reg, int robID){
        registerStat->updateRegister(reg,true,robID);
}
void DecodeUtility::decodeUtility(int reg, int& V,int& Q){
        if(registerStat->registerBusy(reg)){
                int h = registerStat->getRegisterReorderEntryID(reg);
                if(rob->state(h) == ROB_WRITE_RESULT){
                        V = rob->value(h);
                        Q = -1;
                }else{
                        Q = h;
                }
        }else{
                V = registerFile->getRegisterValue(reg);
                Q = -1;
        }
}
RSEntry* DecodeUtility::decodeRTypeInstruction(R_Instruction* instruction, int cycle){
        int Qj,Qk,Vj,Vk;
        decodeUtility(instruction->rs, Vj, Qj);
        decodeUtility(instruction->rt, Vk, Qk);

        ROBEntry* robEntry 
                = new ROBEntry(true, instruction, ROB_EXECUTE, instruction->rd);
        int robID = rob->push(robEntry);
        
        markRegBusy(instruction->rd, robID);
        int numCycles = 1;

        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Vk, Qj, Qk, cycle, numCycles);
        reservationStation->updateROBId(robID);

        return reservationStation; 
}
RSEntry* DecodeUtility::decodeBranchInstruction(I_Instruction* instruction, int cycle){
        int Qj, Vj, Qk, Vk;
        decodeUtility(instruction->rs, Vj, Qj);
        decodeUtility(instruction->rt, Vk, Qk);

        ROBEntry* robEntry                                                                                                     
                = new ROBEntry(true, instruction, ROB_EXECUTE);
        int robID = rob->push(robEntry);
        
        int numCycles = 1;
        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Vk, Qj, Qk, cycle, numCycles);
        reservationStation->updateROBId(robID);

        return reservationStation; 
}

RSEntry* DecodeUtility::decodeITypeInstruction(I_Instruction* instruction, int cycle){
        int Qj,Vj;
        decodeUtility(instruction->rs, Vj, Qj);

        ROBEntry* robEntry                                                                                                     
                = new ROBEntry(true, instruction, ROB_EXECUTE, instruction->rt);
        int robID = rob->push(robEntry);
        
        markRegBusy(instruction->rt, robID);
        int numCycles = 1;

        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Qj, cycle, numCycles);
        reservationStation->updateROBId(robID);
        return reservationStation; 
}
RSEntry* DecodeUtility::decodeLoadInstruction(Lw* instruction, int cycle){
        int Qj, Vj;
        decodeUtility(instruction->rs, Vj, Qj);
        ROBEntry* robEntry                                                                                                     
                = new ROBEntry(true, instruction, ROB_EXECUTE, instruction->rt);
        int robID = rob->push(robEntry);
        
        markRegBusy(instruction->rt, robID);
        int numCycles = 2;
        
        int A = instruction->getImmediate();
        cout<<"Load Instruction decode and A value :"<<A<<endl;
        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Qj, A, cycle, numCycles);
        reservationStation->updateROBId(robID);

        return reservationStation; 
}
RSEntry* DecodeUtility::decodeStoreInstruction(Sw* instruction, int cycle){
        int Qj, Vj, Qk, Vk;
        decodeUtility(instruction->rs, Vj, Qj);
        decodeUtility(instruction->rt, Vk, Qk);
        
        cout<<"Vj = "<<Vj<<"Qj :"<<Qj<<endl;
        ROBEntry* robEntry                                                                                                     
                = new ROBEntry(true, instruction, ROB_EXECUTE);
        int robID = rob->push(robEntry);

        //markRegBusy(instruction->rt, robID);
        int numCycles = 1;

        int A = instruction->getImmediate();
        RSEntry* reservationStation = new RSEntry(instruction, true, Vj, Vk, Qj, Qk, A, cycle, numCycles);
        reservationStation->updateROBId(robID);

        return reservationStation; 
}
RSEntry* DecodeUtility::decodeJTypeInstruction(J_Instruction* instruction, int cycle){
        ROBEntry* robEntry                                                                                                     
                = new ROBEntry(true, instruction, ROB_EXECUTE);
        int robID = rob->push(robEntry);

        int numCycles = 1;
        RSEntry* reservationStation = new RSEntry(instruction, true, cycle, numCycles);
        reservationStation->updateROBId(robID);
        
        return reservationStation; 
}
RSEntry* DecodeUtility::decodeInstruction(Instruction* instruction,int cycle){
        if(instruction->isLoad())
                return decodeLoadInstruction((Lw*)instruction, cycle);
        else if(instruction->isStore())
                return decodeStoreInstruction((Sw*)instruction, cycle);
        else if(instruction->isBranch())
                return decodeBranchInstruction((I_Instruction*)instruction, cycle);
        else if(instruction->getType() == ITYPE)
                return decodeITypeInstruction((I_Instruction*)instruction, cycle);
        else if(instruction->getType() == RTYPE)
                return decodeRTypeInstruction((R_Instruction*)instruction,cycle);
        else if(instruction->getType() == JTYPE)
                return decodeJTypeInstruction((J_Instruction*)instruction, cycle);
}
void DecodeUtility::decodeNOPAndBreak(Instruction* instruction, int cycle){
        ROBEntry* robEntry 
                = new ROBEntry(true, instruction, ROB_EXECUTE);
        int robID = rob->push(robEntry);

        rob->update(robID, 0, cycle, ROB_COMMIT); 
}
