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
                        ofs<<memory_iterator->second->print(true)<<"\r\n";
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
/**
 * Dirty argument parsing.
 */
bool isValid(int num_args, char* argv[],int& m,int& n){
        struct stat buffer;
        if(stat (argv[1], &buffer) == 0){
                if(strcmp(argv[3],"dis") == 0 && num_args == 4)
                        return true;
                else if(strcmp(argv[3],"sim") ==0){
                        if(num_args == 5){
                                try{
                                        std::string opt = argv[4];
                                        if((opt.substr(0,2)).compare("-T") == 0
                                                        && opt.find(":")!= std::string::npos){
                                                string stm = opt.substr(2,opt.find(":")-2);
                                                string stn = opt.substr(opt.find(":")+1,opt.length()-opt.find(":")-1);
                                                m = std::stoi(stm);
                                                n = std::stoi(stn);
                                                if(m<=n)
                                                        return true;
                                        }
                                }catch(std::invalid_argument arg){
                                        return false;
                                }
                        }else
                                return true;
                }
        }
        return false;
}
int main(int argc, char* argv[]){
        int m=0,n=0;
        if(argc < 4 || argc>5 || !isValid(argc,argv,m,n)){
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
        if(strcmp(argv[3],"dis") == 0){
                mipssim.print_memory();
        }else if(strcmp(argv[3],"sim") ==0 ){
                mipssim.print_cycle(mipssim.simulate(m,n));
        }
}
