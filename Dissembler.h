#ifndef _DISSEMBLER_H_
#define _DISSEMBLER_H_
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include "Instructions.h"
using namespace std;
class Dissembler{
        private:
                char* input_file;
                static const string J_TYPE_FORMAT;
                static const string R_OR_BREAK;
                static const string BREAK_I;

                static const string FIVE_ZERO;
                static const string SIX_ZERO;

                //R-Type Instruction functions
                static const string SLTU;
                static const string SUB;
                static const string SUBU;
                static const string ADD;
                static const string ADDU;
                static const string AND;
                static const string OR;
                static const string XOR;
                static const string NOR;
                static const string SLT;
                static const string NOP;

                //I-Type Instruction Formats
                static const string SW;
                static const string LW;
                static const string BEQ;
                static const string BNE;
                static const string ADDI;
                static const string ADDIU;
                static const string SLTI;
                /**
                 * Convert instruction read to binary string.
                 */
                string in_binary(char* instruction_read);

                bool isSLL(string binary_instruction);
                bool isSRL(string binary_instruction);
                bool isSRA(string binary_instruction);
                bool isSLTU(string binary_instruction);
                bool isSUB(string binary_instruction);
                bool isSUBU(string binary_instruction);
                bool isADD(string binary_instruction);
                bool isADDU(string binary_instruction);
                bool isAND(string binary_instruction);
                bool isOR(string binary_instruction);
                bool isXOR(string binary_instruction);
                bool isNOR(string binary_instruction);
                bool isNOP(string binary_instruction);
                bool isSLT(string binary_instruction);

                R_Instruction* parse_R_Type_Instruction(string binary_instruction, int memory);
                bool isSW(string binary_instruction);
                bool isLW(string binary_instruction);
                bool isBEQ(string binary_instruction);
                bool isBNE(string binary_instruction);
                bool isBGEZ(string binary_instruction);
                bool isBGTZ(string binary_instruction);
                bool isBLEZ(string binary_instruction);
                bool isBLTZ(string binary_instruction);
                bool isADDI(string binary_instruction);
                bool isADDIU(string binary_instruction);
                bool isSLTI(string binary_instruction);
                /**
                 * Parse binary instruction in string to sub-type of I-type and return
                 * object to I_Instruction.
                 */
                I_Instruction* parse_I_Type_Instruction(string binary_instruction, int memory);

                Instruction* parse_opcode(string binary_instruction, bool &is_data, int memory);
        public:
                Dissembler();
                //Method to read binary input file
                void read_file(char*input_file, map<int,Abstract*>* memory);
                //Write output to File.
                //void writeOutput(vector<Abstract*> instruct_list);
};
#endif
