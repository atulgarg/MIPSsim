#ifndef _DECODEUTILITY_H_
#define _DECODEUTILITY_H_

#include "Instructions.h"
#include "PipelineUnits.h"
#include "ReservationStation.h"
#include "ROB.h"
class DecodeUtility{
        RegisterStat* registerStat;
        RegisterFile* registerFile;
        ROB* rob;
        void decodeUtility(int reg, int& Vj,int& Qj);
        RSEntry* decodeRTypeInstruction(R_Instruction* instruction, int cycle);
        RSEntry* decodeITypeInstruction(I_Instruction* instruction, int cycle);
        RSEntry* decodeJTypeInstruction(J_Instruction* instruction, int cycle);
        RSEntry* decodeBreakTypeInstruction(BREAK* instruction, int cycle);
        RSEntry* decodeBranchInstruction(I_Instruction* instruction, int cycle);
        RSEntry* decodeLoadInstruction(Lw* instruction, int cycle);
        RSEntry* decodeStoreInstruction(Sw* instruction, int cycle);
        void markRegBusy(int regID, int robID);
        public:
        DecodeUtility(RegisterStat* registerStat, RegisterFile* registerFile, ROB* rob);
        RSEntry* decodeInstruction(Instruction* instruction,int cycle);
        void decodeNOPAndBreak(Instruction* instruction, int cycle);
};
#endif
