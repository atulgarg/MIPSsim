#include "Pipeline.h"
void ALUUnit::execute(RSEntry* reservationStation){
        Instruction* instruction = reservationStation->instruction;
        if(instruction->isLoad()){
                if(reservationStation->numCycles == 2){
                        //address calculation
                }else{
                        //memory access
                }
        }else if(instruction->isStore()){
                if(reservationStation->numCycles == 2){

                }else{

                }
        }else{
                int result = instruction->execute(reservationStation->Vj,reservationStation->Vk);
                reservationStation->result = result;
        }        
        reservationStation->numCycles = reservationStation->numCycles -1;
        //based on results output update value in reservation station.
}
Pipeline::Pipeline(int numReservationStations, int numROBEntry, int numberOfRegister, 
                int numBTBEntries, map<int,Abstract*>* memory_map)
        : reservationStations(numReservationStations), rob(numROBEntry), 
          registerStat(numberOfRegister), registerFile(numberOfRegister),
          decodeUtility(&registerStat,&registerFile,&rob),
          btb(numBTBEntries){
                  //As mentioned in instructions.
                  PC = 600;
                  this->memory_map = memory_map;
          }

vector<string> Pipeline::printPipeline(){
        cout<<"printPipeline"<<endl;
        vector<string> pipelineInstructions;
        pipelineInstructions.push_back("IQ:");
        deque<pair<int,Instruction*> >::iterator instructionQueueIter = instruction_queue.begin();
        for(;instructionQueueIter!=instruction_queue.end();++instructionQueueIter){
                pipelineInstructions.push_back(instructionQueueIter->second->print());
        }

        pipelineInstructions.push_back("RS:");
        vector<string> reservationsStationInstructions = reservationStations.print();
        pipelineInstructions.insert(pipelineInstructions.end(),
                        reservationsStationInstructions.begin(),
                        reservationsStationInstructions.end());

        return pipelineInstructions;
}
void Pipeline::flushCDB(){
        CDB.clear();
}
void Pipeline::instructionFetch(int cycle){
        if(PC < 716){
                Instruction* nextInstruction = (Instruction*)memory_map->find(PC)->second;
                if(nextInstruction!=NULL){
                        if(nextInstruction->isBranch()){
                                NPC = btb.predict(PC);
                                predictedAddress[nextInstruction] = NPC;
                        }else
                                NPC = PC + 4;
                        instruction_queue.push_back(make_pair(cycle,(Instruction*)nextInstruction));
                }else{
                        cout<<"No more instructions in memory"<<endl;
                }
        }
}
void Pipeline::decodeAndIssue(int cycle){
        //check if there is empty reservation station and rob entry 
        if(!instruction_queue.empty() && !rob.isFull()
                        && instruction_queue.front().first < cycle) {
                cout<<"ROB not full cycle:"<<cycle<<endl;
                Instruction* nextInstruction = instruction_queue.front().second;
                if(nextInstruction->isBreak() || nextInstruction->isNOP()){
                        cout<<"Break or NOP cycle:"<<cycle<<endl;
                        instruction_queue.pop_front();
                        ROBEntry* robEntry 
                                = new ROBEntry(true, nextInstruction, ROB_EXECUTE, nextInstruction->getDestination());
                        int robID = rob.push(robEntry);
                        rob.update(robID, 0, cycle, ROB_COMMIT);
                }else if(!reservationStations.isFull()){
                        cout<<"Reservation Station not full cycle:"<<cycle<<endl;
                        instruction_queue.pop_front();
                        //rs reservation station fetch from station
                        RSEntry* reservationStationEntry 
                                = decodeUtility.decodeInstruction(nextInstruction, cycle);
                        reservationStations.addStation(reservationStationEntry);
                        ROBEntry* robEntry 
                                = new ROBEntry(true, nextInstruction, ROB_EXECUTE, 
                                                nextInstruction->getDestination());
                        int robID = rob.push(robEntry);
                        reservationStationEntry->updateReorderID(robID);
                }
        }else{
                cout<<"Instruction Queue empty or rs full or rob full or no instruction for this cycle."<<endl;
        }
}
void Pipeline::execute(int cycle){
        reservationStations.updateStations(CDB);
        flushCDB();
        vector<RSEntry*> toBeExecuted = reservationStations.checkPendingReservationStations(cycle);
        //execute instructions in toBeExecuted.
        for(int i=0;i<toBeExecuted.size();i++){
                RSEntry* rs = toBeExecuted.at(i);
                aluUnit.execute(rs);
                if(rs->getRemainingCycles() == 0){
                        //execute then check if complete to move it to next stage.
                        executedInstruction.push_back(make_pair(rs,cycle));
                }
        }
}
void Pipeline::writeResult(int cycle){
        //write result to ROB and waiting RS through CDB
        for(int i=0;i<executedInstruction.size();i++){
                if(executedInstruction.at(i).second == cycle){
                        Instruction* instruction = executedInstruction.at(i).first->instruction;
                        if(instruction->isStore()){

                        }else if(instruction->isJump() || instruction->isBranch()){
                                //enter rob with same cycle no so as will be executed
                                RSEntry* completedStation = executedInstruction.at(i).first;
                                int result = completedStation->result;
                                int robID = completedStation->getReorderID();
                                reservationStations.remove(completedStation);
                                //computed value to ROB.
                                rob.update(robID, result, cycle, ROB_COMMIT);
                        }
                        // && instruction is of Store JUMP or branch
                        //skip this to next stage
                }
                if(executedInstruction.at(i).second < cycle){
                        RSEntry* completedStation = executedInstruction.at(i).first;
                        int result = completedStation->result;
                        int robID = completedStation->getReorderID();
                        reservationStations.remove(completedStation);
                        //computed value to ROB.
                        rob.update(robID, result, cycle, ROB_COMMIT);
                        //write results to CDB
                        CDB[robID] = result;
                }
        }
        //Store, Jump Branch NOP and break skip this stage.
}
bool Pipeline::commit(int cycle){
        if(!rob.isEmpty() 
                        && rob.peek()->getState() == ROB_COMMIT 
                        && rob.peek()->getCycle() < cycle){
                ROBEntry* robHead = rob.peek();
                int destination = robHead->getDestination();
                if(robHead->getInstruction()->isBranch()){
                       if(robHead->getValue() != predictedAddress[robHead->getInstruction()]){
                               //misprediction
                       } 
                }else if(robHead->getInstruction()->isStore()){
                        memory_map->insert(make_pair(destination,new Abstract(robHead->getValue(),destination)));
                }else{
                        registerFile.setRegisterValue(destination, robHead->getValue());  
                }
                rob.pop();
                if(registerStat.getRegisterReorderEntryID(destination) == rob.getHeadID())
                        registerStat.updateRegister(destination, false, -1);
        }
        //Regular ALU instruction
        //updates the register with result and remove the instruction from ROB and RS
        //Store Commit
        //main memory is updated instead of register updation
}
