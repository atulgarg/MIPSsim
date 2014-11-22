#ifndef _MIPSSIM_H_
#define _MIPSSIM_H_
#include<iostream>
#include<map>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include "Dissembler.h"
#include "Simulator.h"
#include "Instructions.h"
using namespace std;
class MIPSsim{
        Dissembler* dissembler;
        Simulator* simulator;
        std::map<int,Abstract*>* memory;
        char* input_file;
        char* output_file;
        public:
        MIPSsim(char* inputfile, char* outputfile);
        void dissemble();
        void simulate(int m,int n);
        void print_memory();
};
#endif
