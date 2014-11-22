#include "Pipeline.h"
Pipeline::Pipeline(int numReservationStations, int numROBEntry, int numberOfRegister)
        : reservationStations(numReservationStations), rob(numROBEntry), 
          registerStat(numberOfRegister), registerFile(numberOfRegister){
        //As mentioned in instructions.
        PC = 600;
}
void Pipeline::instructionFetch(int cycle){
        Abstract* nextInstruction = memory_map[PC];
        if(nextInstruction!=NULL){
                //TODO increament PC
                this->PC = this->PC + 4;
                instruction_queue.push(make_pair(cycle,(Instruction*)nextInstruction));
                //TODO BTB implementation.
        }else{
                cout<<"No more instructions in memory"<<endl;
        }
}

void Pipeline::decodeAndIssue(int cycle){
        //check if there is empty reservation station and rob entry 
        if(!instruction_queue.empty() 
                        && !reservationStations.isFull()
                        && !rob.isFull()
                        && instruction_queue.top().first < cycle) {
                Instruction* nextInstruction = instruction_queue.top().second;
                instruction_queue.pop();
                //rs reservation station fetch from station
                RSEntry* reservationStationEntry 
                        = nextInstruction->decodeInstruction(registerStat, registerFile, rob, cycle);
                reservationStations.addStation(reservationStationEntry);
                ROBEntry* robEntry 
                        = new ROBEntry(true, nextInstruction, ROB_EXECUTE, nextInstruction->getDestination());
                int robID = rob.push(robEntry);
                reservationStationEntry->updateReorderID(robID);
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
