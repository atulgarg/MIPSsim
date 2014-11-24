#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_
#include<string>
#include<cmath>
#include<iostream>
#include<sstream>
#include<climits>
using namespace std;
enum Instruction_Type {ITYPE, RTYPE , JTYPE, DATATYPE, BREAKTYPE, NOPTYPE};
class Abstract{
        string binary_instruction;
        int memory;
        Instruction_Type instructionType;
        int value;
        public:
        Abstract(string binary_instruction, int memory,Instruction_Type instructionType){
                this->binary_instruction = binary_instruction;
                this->memory = memory;
                this->instructionType = instructionType;
                this->value = convert_2_s_complement_to_int_16_32(binary_instruction);
        }
        Abstract(int value, int memory){
                this->value = value;
                this->memory = memory;
                this->instructionType = DATATYPE;
        }
        virtual string print(){
                stringstream ss;
                ss<<binary_instruction <<" "<<memory<<" "<<value;
                return ss.str();
        }
        string getInstruction(){
                return binary_instruction;
        }
        int getMemory(){
                return memory;
        }
        int convert_binary_string_to_int(string string_val){
                double val = 0;
                for(int i=string_val.length()-1,j=0;i>=0;i--,j++){
                        if(string_val.at(i) == '1')
                                val = val + pow(2,j);
                }
                return (int) val;
        }
        int convert_2_s_complement_to_int_16_32(string string_val){
                stringstream ss;
                if(string_val[0] == '1'){  
                        //complement bits
                        for(int i=0;i<string_val.length();i++){
                                if(string_val[i] == '1')
                                        ss<<'0';
                                else
                                        ss<<'1';
                        }
                        int decimal = (convert_binary_string_to_int(ss.str()));
                        if((string_val.length() == 32 && decimal == INT_MIN) || (string_val.length() == 16 && decimal == 65535))
                                decimal = -1;
                        return (decimal + 1)* -1;
                }else
                        return convert_binary_string_to_int(string_val);
        }
        Instruction_Type getType(){
                return instructionType;
        }
};
class Instruction: public Abstract{
        public:
                Instruction(string instruction, int memory, Instruction_Type instructionType)
                        : Abstract(instruction, memory, instructionType){
                        }
                virtual string print(){
                        stringstream ss;
                        ss<<getInstruction().substr(0,6)<<" "
                                << getInstruction().substr(6,5) << " "
                                << getInstruction().substr(11,5) << " "
                                << getInstruction().substr(16,5) << " "
                                << getInstruction().substr(21,5) << " "
                                << getInstruction().substr(26,6) << " "
                                << getMemory();
                        return ss.str();
                }
                virtual int getDestination(){
                        //TODO virtual
                }
                virtual bool isStore(){
                        return false;
                }
                virtual bool isLoad(){
                        return false;
                }
                virtual bool isBranch(){
                        return false;
                }
                virtual bool isNOP(){
                        return false;
                }
                virtual bool isBreak(){
                        return false;
                }
                virtual bool isJump(){
                        return false;
                }
                virtual int execute(int rs, int rt){
                        cout<<"Should not come here ever"<<endl;
                }
};
class R_Instruction : public Instruction{
        public:
                friend class DecodeUtility;
                friend class ALUUnit;
                short int rs; 
                short int rt; 
                short int rd; 
                short int sd; 
                short int function;
                R_Instruction(string instruction, int memory): Instruction(instruction, memory, RTYPE){
                        this->rs = convert_binary_string_to_int(instruction.substr(6,5));
                        this->rt = convert_binary_string_to_int(instruction.substr(11,5));
                        this->rd = convert_binary_string_to_int(instruction.substr(16,5));
                        this->sd = convert_binary_string_to_int(instruction.substr(21,5));
                        this->function = convert_binary_string_to_int(instruction.substr(26,6));
                }
                int getDestination(){
                        return rd;
                }
};
class I_Instruction : public Instruction{
        public:
                friend class ALUUnit;
                int rs;
                int rt;
                int immediate;
                I_Instruction(string instruction, int memory): Instruction(instruction, memory, ITYPE){
                        this->rs = convert_binary_string_to_int(instruction.substr(6,5));
                        this->rt = convert_binary_string_to_int(instruction.substr(11,5));
                        this->immediate = convert_2_s_complement_to_int_16_32(instruction.substr(16,16));
                }
                virtual int getImmediate(){
                        return immediate;
                }
                int getDestination(){
                        return rt;
                }
};
class J_Instruction : public Instruction{
        private:
                int target;
                public:
                friend class ALUUnit;
                J_Instruction(string instruction, int memory): Instruction(instruction, memory, JTYPE){
                        this->target = convert_binary_string_to_int(instruction.substr(6,26))<<2;
                }
                virtual string print(){
                        stringstream ss;
                        ss<<Instruction::print()<<" J #"<<target;
                        return ss.str();
                }
                bool isJump(){
                        return true;
                }
                virtual int execute(){
                        
                }
};
class BREAK: public Instruction{
        public:
                friend class ALUUnit;
                BREAK(string instruction, int memory): Instruction(instruction, memory, BREAKTYPE){
                }   
                virtual string print(){
                        return  Instruction::print() + " BREAK";
                }
                bool isBreak(){
                        return true;
                }
};
//R Type instructions
class Sll: public R_Instruction{
        public:
                Sll(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SLL R" << rd << ", R" << rt << ", #" << sd;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return rt<<sd;
                }
};
class Srl: public R_Instruction{
        public:
                Srl(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SRL R" << rd << ", R" << rt << ", #" << sd;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return rt>>sd;
                }
};
class Sra: public R_Instruction{
        public:
                Sra(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SRA R" << rd << ", R" << rt << ", #" << sd;
                        return ss.str();
                }
                virtual int execute(int rs,int rt){
                        //TODO sign
                        return rt >> sd; 
                }
};
class Sub: public R_Instruction{
        public:
                Sub(string instruction, int memory): R_Instruction(instruction,memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SUB R" << rd << ", R" << rs << ", " << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        //TODO unsigned overflow.
                        return (rs-rt);
                }
};
class Sltu: public R_Instruction{
        public:
                Sltu(string instruction, int memory): R_Instruction(instruction,memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SLTU R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        //TODO unsigned comparison.
                        if(rs < rt)
                                return 1;
                        return 0;
                }
};
class Subu: public R_Instruction{
        public:
                Subu(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SUBU R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return rs-rt;
                }
};
class Add: public R_Instruction{
        public:
                Add(string instruction, int memory): R_Instruction(instruction,memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " ADD R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                //TODO overflow??
                virtual int execute(int rs, int rt){
                        return rs + rt;
                }
};
class Addu: public R_Instruction{
        public:
                Addu(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " ADDU R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return rs + rt;
                }
};
class And: public R_Instruction{
        public:
                And(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " AND R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return (rs & rt);
                }
};
class Or: public R_Instruction{
        public:
                Or(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " OR R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return (rs | rt);
                }
};
class Xor: public R_Instruction{
        public:
                Xor(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " XOR R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return rs^rt;
                }
};
class Nor: public R_Instruction{
        public:
                Nor(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " NOR R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return (~(rs|rt));
                }
};
class Nop: public Instruction{
        public:
                Nop(string instruction, int memory): Instruction(instruction, memory, NOPTYPE){
                }
                virtual string print(){
                        return Instruction::print() + " NOP";
                }
                bool isNOP(){
                        return true;
                }
};
class Slt: public R_Instruction{
        public:
                Slt(string instruction, int memory): R_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SLT R" << rd << ", R" << rs << ", R" << rt;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        if(rs < rt)
                                return 1;
                        return 0;
                }
};
//I Type Instructions
class Sw: public I_Instruction{
        public:
                Sw(string instruction, int memory): I_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SW R" << rt << ", " << immediate << "(R" << rs << ")";
                        return ss.str();
                }
                bool isStore(){
                        return true;
                }
};
class Lw: public I_Instruction{
        public:
                Lw(string instruction, int memory): I_Instruction(instruction, memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " LW R" << rt << ", " << immediate << "(R" << rs << ")";
                        return ss.str();
                }
                bool isLoad(){
                        return true;
                }
                
};
class Beq: public I_Instruction{
        public:
                Beq(string instruction, int memory): I_Instruction(instruction, memory){
                }
                virtual int getImmediate(){
                        return immediate<<2;
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " BEQ R" << rs << ", R" << rt << ", #" <<getImmediate();
                        return ss.str();
                }
                virtual int execute(int rs,int rt){
                        if(rs == rt)
                                return getMemory() + 4 + getImmediate();
                        else
                                return getMemory() + 4;
                }
                bool isBranch(){
                        return true;
                }
};
class Bne: public I_Instruction{
        public:
                Bne(string instruction, int memory): I_Instruction(instruction, memory){
                }
                virtual int getImmediate(){
                        return immediate<<2;
                }

                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " BNE R" << rs << ", R" << rt << ", #" << getImmediate();
                        return ss.str();
                }
                bool isBranch(){
                        return true;
                }
                virtual int execute(int rs, int rt){
                        if(rs != rt)
                                return getMemory() + 4 + getImmediate();
                        else
                                return getMemory() + 4;

                }

};
class Bgez: public I_Instruction{
        public:
                Bgez(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual int getImmediate(){
                        return immediate<<2;
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " BGEZ R" << rs << ", #" << getImmediate();
                        return ss.str();
                }
                bool isBranch(){
                        return true;
                }
                virtual int execute(int rs, int rt){
                        if(rs >= 0)
                                return getMemory() + 4 + getImmediate();
                        else
                                return getMemory() + 4;
                }

};
class Bgtz: public I_Instruction{
        public:
                Bgtz(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual int getImmediate(){
                        return immediate<<2;
                }
                virtual string print() {
                        stringstream ss;
                        ss<< Instruction::print() << " BGTZ R" << rs << ", #" << getImmediate();
                        return ss.str();
                }
                bool isBranch(){
                        return true;
                }
                virtual int execute(int rs, int rt){
                        if(rs > 0)
                                return getMemory() + 4 + getImmediate();
                        else
                                return getMemory() + 4;
                }

};
class Blez: public I_Instruction{
        public:
                Blez(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual int getImmediate(){
                        return immediate<<2;
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " BLEZ R" << rs << ", #" << getImmediate();
                        return ss.str();
                }
                bool isBranch(){
                        return true;
                }
                virtual int execute(int rs, int rt){
                        if(rs <= 0)
                                return getMemory() + 4 + getImmediate();
                        else
                                return getMemory() + 4;
                }

};
class Bltz: public I_Instruction{
        public:
                Bltz(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual int getImmediate(){
                        return immediate<<2;
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " BLTZ R" << rs << ", #" <<getImmediate();
                        return ss.str();
                }
                bool isBranch(){
                        return true;
                }
                virtual int execute(int rs, int rt){
                        if(rs < 0)
                                return getMemory() + 4 + getImmediate();
                        else
                                return getMemory() + 4;
                }

};
class Addi: public I_Instruction{
        public:
                Addi(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " ADDI R" << rt << ", R" << rs << ", #" << immediate;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        //TODO overflow
                        return rs + getImmediate();
                }
};
class Addiu: public I_Instruction{
        public:
                Addiu(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " ADDIU R" << rt << ", R" << rs << ", #" << immediate;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        return rs+ getImmediate();
                }
};
class Slti: public I_Instruction{
        public:
                Slti(string instruction, int memory): I_Instruction(instruction,memory){
                }
                virtual string print(){
                        stringstream ss;
                        ss<< Instruction::print() << " SLTI R" << rt << ", R" << rs << ", #" << immediate;
                        return ss.str();
                }
                virtual int execute(int rs, int rt){
                        if(rs < immediate)
                                return 1;
                        return 0;
                }
};
#endif
