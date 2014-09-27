#ifndef _MIPSSIM_H_
#define _MIPSSIM_H_
#include<iostream>
#include "Dissembler.h"
#include "Instructions.h"
class MIPSsim{
        Dissembler* dissembler;
        vector<Abstract*> instruction_list;
        char* input_file;
        char* output_file;
        public:
        MIPSsim(char* inputfile, char* outputfile);
        void dissemble();
};
#endif
