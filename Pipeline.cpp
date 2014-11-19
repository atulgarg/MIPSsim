#include "Pipeline.h"
Pipeline::Pipeline(){
        //As mentioned in instructions.
        ALU_Units = 10;
        PC = 600;
}
void Pipeline::instructionFetch(){
        Abstract* next_instruction = memory[PC];
        if(next_instruction!=NULL){
                //TODO increament PC
                this->PC = this->PC + 4;
                instruction_queue.push_back(next_instruction);
                //TODO BTB implementation.
        }else{
                cout<<"No more instructions in memory"<<endl;
        }
}

void Pipeline::decodeAndIssue(){
        if(!instruction_queue.empty() && !reservationStations.isFull() && !rob.isFull()) {     //&&empty reservation station and empty rob)
                //check if there is empty reservation station and rob entry 
                Abstract* next_instruction = instruction_queue.pop_front();
                //rs reservation station fetch from station
                RSEntry* rs = reservationStations.getReservationStation();
                //robID fetch from ROB
                rs->updateReorderID(robID);
                rs->setBusy();
        }else{
                cout<<"Instruction Queue empty."<<endl;
        }
}
void Pipeline::execute(){
        //monitor CDB for operands not yet available.
        //checks RAW hazards.
        //execute instruction
        if(ALU_Units > 0){

        }
}
void Pipeline::writeResult(){
        //write result to ROB and waiting RS through CDB
        //Store, Jump Branch NOP and break skip this stage.
        //change robStatus ready to commit.
}
void Pipeline::commit(){
        //Regular ALU instruction
        //updates the register with result and remove the instruction from ROB and RS
        //Store Commit
        //main memory is updated instead of register updation
}
