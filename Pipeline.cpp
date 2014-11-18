#include "Pipeline.h"
Pipeline::Pipeline(){
        PC = 600;
}
void Pipeline::instructionFetch(){
        Abstract* next_instruction = memory[PC];
        if(next_instruction!=NULL){
                //TODO increament PC
                instruction_queue.push_back(next_instruction);
                //TODO BTB implementation.
                //
        }else{
                cout<<"No more instructions in memory"<<endl;
        }
}

void Pipeline::decodeAndIssue(){
        if(!instruction_queue.empty() {     //&&empty reservation station and empty rob)
                Abstract* next_instruction = instruction_queue.pop_front();
                //rs reservation station fetch from station
                //robID fetch from ROB
                rs->updateReorderID(robID);
                rs->setBusy();
        }else{
                cout<<"Instruction Queue empty."<<endl;
        }
}
void Pipeline::execute(){

}
void Pipeline::writeResult(){

}
void Pipeline::commit(){

}
