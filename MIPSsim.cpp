#include "MIPSsim.h"
using namespace std;
MIPSsim::MIPSsim(char* input_file,char* output_file)
{
        this->input_file = input_file;
        this->output_file = output_file;
        this->memory = new map<int,Abstract*>();
        this->dissembler = new Dissembler();
        this->simulator = new Simulator(memory, 600,10,6,32,16);
}
void MIPSsim::dissemble(){
        //initialise memory by dissembling the instructions and storing in memory.
        this->dissembler->read_file(input_file, memory);
}
vector<string> MIPSsim::simulate(int m, int n){
        if(m == n && m == 0)
                return simulator->simulate();
        else
                return simulator->simulate(m,n);
}
/**
 * Utility function to print content of memory.
 */
void MIPSsim::print_memory(){
        if(memory!=NULL){
                //iterate through ordered map and print the content.
                map<int, Abstract*>::iterator memory_iterator = memory->begin();
                ofstream ofs(output_file,ofstream::out);
                for(;memory_iterator!=memory->end();++memory_iterator){
                        ofs<<memory_iterator->second->print()<<"\r\n";
                }
                ofs.close();
        }
}
void MIPSsim::print_cycle(vector<string> cycleOutput){
        ofstream ofs(output_file,ofstream::out);
        for(int i=0;i<cycleOutput.size();i++){
                ofs<<cycleOutput.at(i)<<"\r\n";
        }
        ofs.close();
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
        //this will initialise memory.
        mipssim.dissemble();
        if(strcmp(argv[3],"dis") == 0){
                mipssim.print_memory();
        }else if(strcmp(argv[3],"sim") ==0 ){
                //TODO m,n parse
                mipssim.print_cycle(mipssim.simulate(0,0));
        }
}
