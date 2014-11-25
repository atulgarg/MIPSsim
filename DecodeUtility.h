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
        RSEntry* decodeLoadStoreInstruction(I_Instruction* instruction, int cycle);
        public:
        DecodeUtility(RegisterStat* registerStat, RegisterFile* registerFile, ROB* rob);
        RSEntry* decodeInstruction(Instruction* instruction,int cycle);
};
#endif
