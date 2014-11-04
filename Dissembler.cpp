#include "Dissembler.h"
const string Dissembler::J_TYPE_FORMAT =  "000010";
const string Dissembler::R_OR_BREAK = "000000";
const string Dissembler::BREAK_I = "00000000000000000000000000001101";

const string Dissembler::FIVE_ZERO = "00000";
const string Dissembler::SIX_ZERO = "000000";

const string Dissembler::SLTU = "00000101011";
const string Dissembler::SUB  = "00000100010";
const string Dissembler::SUBU = "00000100011";
const string Dissembler::ADD = "00000100000";
const string Dissembler::ADDU = "00000100001";
const string Dissembler::AND = "00000100100";
const string Dissembler::OR  = "00000100101";
const string Dissembler::XOR = "00000100110";
const string Dissembler::NOR = "00000100111";
const string Dissembler::SLT = "00000101010";
const string Dissembler::NOP = "00000000000000000000000000000000";

const string Dissembler::SW = "101011";
const string Dissembler::LW = "100011";
const string Dissembler::BEQ = "000100";
const string Dissembler::BNE = "000101";
const string Dissembler::ADDI = "001000";
const string Dissembler::ADDIU = "001001";
const string Dissembler::SLTI = "001010";

Dissembler::Dissembler(){
}
/**
 * Convert instruction read to binary string.
 */
string Dissembler::in_binary(char* instruction_read){
        char *binary_instruction = new char[33];
        int index =0;
        for(int i=0;i<4;i++)
        {
                int num = (int)instruction_read[i];
                for(int j = 7;j>=0;j--)
                {
                        binary_instruction[index++] = ((num>>j) & 1 == 1?'1':'0');
                }
        }
        binary_instruction[index]='\0';
        string bin_instruction_str(binary_instruction);
        return bin_instruction_str;
}

bool Dissembler::isSLL(string binary_instruction){
        return (binary_instruction.substr(6,5).compare(FIVE_ZERO) == 0 
                        && binary_instruction.substr(26,6).compare(SIX_ZERO) == 0);
}
bool Dissembler::isSRL(string binary_instruction){
        return (binary_instruction.substr(6,5).compare(FIVE_ZERO) == 0 
                        && binary_instruction.substr(26,6).compare("000010") == 0);
}
bool Dissembler::isSRA(string binary_instruction){
        return (binary_instruction.substr(6,5).compare(FIVE_ZERO) == 0
                        && binary_instruction.substr(26,6).compare("000011") == 0);
}
bool Dissembler::isSLTU(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(SLTU) == 0);
}
bool Dissembler:: isSUB(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(SUB) == 0);
}
bool Dissembler:: isSUBU(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(SUBU) == 0);
}
bool Dissembler:: isADD(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(ADD) == 0);
}
bool Dissembler:: isADDU(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(ADDU) == 0);
}
bool Dissembler:: isAND(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(AND) == 0);
}
bool Dissembler:: isOR(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(OR) == 0);
}
bool Dissembler:: isXOR(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(XOR) == 0);
}
bool Dissembler:: isNOR(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(NOR) == 0);
}
bool Dissembler:: isNOP(string binary_instruction){
        return (binary_instruction.compare(NOP) == 0);
}
bool Dissembler:: isSLT(string binary_instruction){
        return (binary_instruction.substr(21,11).compare(SLT) == 0);
}
R_Instruction* Dissembler::parse_R_Type_Instruction(string binary_instruction, int memory){
        if(isNOP(binary_instruction)){
                return new Nop(binary_instruction, memory);
        }else if(isSRL(binary_instruction)){
                return new Srl(binary_instruction, memory);
        }else if(isSRA(binary_instruction)){
                return new Sra(binary_instruction, memory);
        }else if(isSLTU(binary_instruction)){
                return new Sltu(binary_instruction, memory);
        }else if(isSUB(binary_instruction)){
                return new Sub(binary_instruction, memory);
        }else if(isSUBU(binary_instruction)){
                return new Subu(binary_instruction, memory);
        }else if(isADD(binary_instruction)){
                return new Add(binary_instruction, memory);
        }else if(isADDU(binary_instruction)){
                return new Addu(binary_instruction, memory);
        }else if(isAND(binary_instruction)){
                return new And(binary_instruction, memory);
        }else if(isOR(binary_instruction)){
                return new Or(binary_instruction, memory);
        }else if(isXOR(binary_instruction)){
                return new Xor(binary_instruction, memory);
        }else if(isNOR(binary_instruction)){
                return new Nor(binary_instruction, memory);
        }else if(isSLL(binary_instruction)){
                return new Sll(binary_instruction, memory);
        }else if(isSLT(binary_instruction)){
                return new Slt(binary_instruction, memory);
        }else{
                cout<<binary_instruction<<endl;
                cout<<"Operation not supported"<<endl;
                exit(0);
        }
}
bool Dissembler::isSW(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(SW) == 0);
}
bool Dissembler::isLW(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(LW) == 0);
}
bool Dissembler::isBEQ(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(BEQ) == 0);
}
bool Dissembler::isBNE(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(BNE) == 0);
}
bool Dissembler::isBGEZ(string binary_instruction){
        return (binary_instruction.substr(0,6).compare("000001") == 0
                        && binary_instruction.substr(11,5).compare("00001")== 0);
}
bool Dissembler::isBGTZ(string binary_instruction){
        return (binary_instruction.substr(0,6).compare("000111") == 0
                        && binary_instruction.substr(11,5).compare(FIVE_ZERO) == 0);
}
bool Dissembler::isBLEZ(string binary_instruction){
        return (binary_instruction.substr(0,6).compare("000110") == 0
                        && binary_instruction.substr(11,5).compare(FIVE_ZERO) == 0);
}
bool Dissembler::isBLTZ(string binary_instruction){
        return (binary_instruction.substr(0,6).compare("000001") == 0
                        && binary_instruction.substr(11,5).compare(FIVE_ZERO) == 0);
}
bool Dissembler::isADDI(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(ADDI) == 0);
}
bool Dissembler::isADDIU(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(ADDIU) == 0);
}
bool Dissembler::isSLTI(string binary_instruction){
        return (binary_instruction.substr(0,6).compare(SLTI) == 0);
}
/**
 * Parse binary instruction in string to sub-type of I-type and return
 * object to I_Instruction.
 */
I_Instruction* Dissembler::parse_I_Type_Instruction(string binary_instruction, int memory){
        if(isSW(binary_instruction)){
                return new Sw(binary_instruction, memory);
        }else if(isLW(binary_instruction)){
                return new Lw(binary_instruction, memory);
        }else if(isBEQ(binary_instruction)){
                return new Beq(binary_instruction, memory);
        }else if(isBNE(binary_instruction)){
                return new Bne(binary_instruction, memory);
        }else if(isBGEZ(binary_instruction)){
                return new Bgez(binary_instruction, memory);
        }else if(isBLTZ(binary_instruction)){
                return new Bltz(binary_instruction, memory);
        }else if(isADDI(binary_instruction)){
                return new Addi(binary_instruction, memory);
        }else if(isADDIU(binary_instruction)){
                return new Addiu(binary_instruction, memory);
        }else if(isSLTI(binary_instruction)){
                return new Slti(binary_instruction, memory);
        }else if(isBLEZ(binary_instruction)){
                return new Blez(binary_instruction, memory);
        }else if(isBGTZ(binary_instruction)){
                return new Bgtz(binary_instruction, memory);
        }
        else{
                cout<<"Unsuported instruction."<<endl;
                exit(0);
        }
}
Instruction* Dissembler::parse_opcode(string binary_instruction, bool &is_data, int memory_address){
        string opcode = binary_instruction.substr(0,6);
        if(opcode.compare(J_TYPE_FORMAT)==0){
                //check if supported type instruction and create one.
                return new J_Instruction(binary_instruction, memory_address);;
        }else if(opcode.compare(R_OR_BREAK) == 0){
                //determine R or Break;
                if(binary_instruction.compare(BREAK_I) == 0){
                        is_data = true;
                        return new BREAK(binary_instruction, memory_address);
                }
                else{
                        //check if supported R_Instruction if yes then create one and return.
                        return parse_R_Type_Instruction(binary_instruction, memory_address);
                }
        }else
                return parse_I_Type_Instruction(binary_instruction, memory_address); 
}
void Dissembler::read_file(char* input_file, map<int,Abstract*>* memory){
        FILE* fp;
        char instruction_read[4];
        fp = fopen(input_file,"rb");
        bool is_data = false;
        int memory_address = 600;
        while(fread(&instruction_read,1,4,fp)!=0)
        {
                string binary_instruction  = in_binary(instruction_read);
                if(!is_data){
                        memory->insert(make_pair(memory_address,parse_opcode(binary_instruction,is_data,memory_address)));
                }else{
                        memory->insert(make_pair(memory_address, (new Abstract(binary_instruction, memory_address))));
                }
                memory_address+=4;
        }
}
/**
 * Write output to File.
void Dissembler::writeOutput(vector<Abstract*> instruct_list){
        ofstream ofs(output_file,ofstream::out);
        for(int i=0;i<instruct_list.size();i++)
                ofs<<instruct_list.at(i)->print()<<"\r\n";
}
*/
