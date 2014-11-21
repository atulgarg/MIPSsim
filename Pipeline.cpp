#include "Pipeline.h"
Pipeline::Pipeline(int numReservationStations, int numROBEntry): reservationStations(numReservationStations),rob(numROBEntry){
        //As mentioned in instructions.
        PC = 600;
}
void Pipeline::instructionFetch(int cycle){
        Abstract* next_instruction = memory_map[PC];
        if(next_instruction!=NULL){
                //TODO increament PC
                this->PC = this->PC + 4;
                instruction_queue.push(make_pair(cycle,next_instruction));
                //TODO BTB implementation.
        }else{
                cout<<"No more instructions in memory"<<endl;
        }
}

void Pipeline::decodeAndIssue(int cycle){
        //check if there is empty reservation station and rob entry 
        if(!instruction_queue.empty() 
                        && !reservationStations.isFull()
                        //                 && !rob.isFull()
                        && instruction_queue.top().first < cycle) {
                Abstract* next_instruction = instruction_queue.top().second;
                instruction_queue.pop();
                //TODO Decode Instruction
                if(next_instruction->getType() == BreakType ){

                }else if(next_instruction->getType() == RType){

                }else if(next_instruction->getType() == IType){

                }else if(next_instruction->getType() == JType){

                }else if(next_instruction->getType() == DataType){

                }
                //rs reservation station fetch from station
                //RSEntry* reservationStation = RSEntry(true,int Vj,int Vk,int ROBId_Qj,int ROBId_Qk,int A, cycle);
                //robID fetch from ROB
        }else{
                cout<<"Instruction Queue empty."<<endl;
        }
}
void Pipeline::execute(int cycle){
        //monitor CDB for operands not yet available.
        //checks RAW hazards.
        //execute instruction
        /*
        if(!reservationStation->isBusy()
                        && reservationStation->cycle < cycle){
        }
        */
}
void Pipeline::writeResult(int cycle){
        //write result to ROB and waiting RS through CDB
        //Store, Jump Branch NOP and break skip this stage.
        //change robStatus ready to commit.
}
void Pipeline::commit(int cycle){
        //Regular ALU instruction
        //updates the register with result and remove the instruction from ROB and RS
        //Store Commit
        //main memory is updated instead of register updation
}
