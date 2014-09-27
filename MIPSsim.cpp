#include<iostream>
#include<cstring>
#include<cstdlib>
#include "MIPSsim.h"
using namespace std;
MIPSsim::MIPSsim(char* input_file,char* output_file)
{
        this->input_file = input_file;
        this->output_file = output_file;
        this->dissembler = new Dissembler(input_file, output_file);
}
void MIPSsim::dissemble(){
        instruction_list = this->dissembler->read_file();
        dissembler->writeOutput(instruction_list);
}

bool notValidArguments(int num_args, char* argv[]){
        if(strcmp(argv[3],"dis") != 0 && strcmp(argv[3],"sim") != 0)
                return true;
        else
                return false;
}

int main(int argc, char* argv[]){
        if(argc < 4 || argc>5 || notValidArguments(argc,argv)){
                cout<<"Incorrect usage."<<endl<<
                        "Usage: MIPSsim inputfilename outputfilename operation [-Tm:n]"<<endl<<endl
                        <<"Inputfilename - The file name of the binary input file."<<endl
                        <<"Outputfilename - The file name to which to print the output."<<endl
                        <<"Operation - Either\"dis\" or\"sim\" to specify disassembly or simulation."<<endl
                        <<"-Tm:n - Optional argument to specify the start (m) and end (n) cycles of simulation tracing output."<<endl;
                exit(0);
        }
        MIPSsim mipssim(argv[1],argv[2]);
        mipssim.dissemble();
}
