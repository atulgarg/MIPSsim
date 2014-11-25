#include "Pipeline.h"
void ALUUnit::execute(RSEntry* reservationStation, ROB* rob,map<int,Abstract*>* memory){
        Instruction* instruction = reservationStation->instruction;
        if(instruction->isLoad() || instruction->isStore()){
                if(reservationStation->numCycles == 2){
                        //address calculation
                        if(rob->allAddressComputedBefore(reservationStation->getROBId())){
                                int memoryAddress = instruction->execute(reservationStation->Vj,reservationStation->Vk);
                                reservationStation->A = memoryAddress;
                                //TODO need to check if it was not updated in the same cycle.
                                reservationStation->numCycles =  reservationStation->numCycles -1;
                        }
                }else{
                        if(instruction->isLoad() && rob->noPendingStore(reservationStation->getROBId())){
                                reservationStation->result = memory->find(reservationStation->A)->second->getValue();
                                reservationStation->numCycles--;
                        }else if(instruction->isStore()){
                                reservationStation->numCycles--;
                        }
                }
        }else{
                reservationStation->result = instruction->execute(reservationStation->Vj,reservationStation->Vk);
                reservationStation->numCycles = reservationStation->numCycles -1;
        }        
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
                pipelineInstructions.push_back(instructionQueueIter->second->print(false));
        }

        pipelineInstructions.push_back("RS:");
        vector<string> reservationsStationInstructions = reservationStations.print();
        pipelineInstructions.insert(pipelineInstructions.end(),
                        reservationsStationInstructions.begin(),
                        reservationsStationInstructions.end());
        
        pipelineInstructions.push_back("ROB:");
        vector<string> ROBInstructions = rob.print();
        pipelineInstructions.insert(pipelineInstructions.end(),
                        ROBInstructions.begin(),
                        ROBInstructions.end());

        pipelineInstructions.push_back("BTB:");
        vector<string> BTBTable = btb.print();
        pipelineInstructions.insert(pipelineInstructions.end(), BTBTable.begin(), BTBTable.end());

        pipelineInstructions.push_back("Registers:");
        vector<string> registerValues = registerFile.print();
        pipelineInstructions.insert(pipelineInstructions.end(), registerValues.begin(), registerValues.end());

        return pipelineInstructions;
}
void Pipeline::resetPipeline(int robID, int PC){
        reservationStations.reset(robID);
        rob.reset(robID);
        CDB.clear();
        instruction_queue.clear();
        this->PC = PC;
}
void Pipeline::instructionFetch(int cycle){
        if(PC < 716){
                cout<<"Instruction Fetch open:"<<cycle<<endl;
                Instruction* nextInstruction = (Instruction*)memory_map->find(PC)->second;
                if(nextInstruction!=NULL){
                        if(nextInstruction->isBranch()){
                                PC = btb.predict(PC);
                                predictedAddress[nextInstruction] = PC;
                        }else
                                PC = PC + 4;
                        instruction_queue.push_back(make_pair(cycle,(Instruction*)nextInstruction));
                }else{
                        cout<<"No more instructions in memory"<<endl;
                }
        }
        cout<<"Instruction Fetch close: "<<cycle<<endl;
}
void Pipeline::decodeAndIssue(int cycle){
        cout<<"Decode and Issue Open"<<cycle<<endl;
        if(!instruction_queue.empty() && !rob.isFull()
                        && instruction_queue.front().first < cycle) {
                Instruction* nextInstruction = instruction_queue.front().second;
                ROBEntry* robEntry 
                        = new ROBEntry(true, nextInstruction, ROB_EXECUTE, nextInstruction->getDestination());

                if(nextInstruction->isBreak() || nextInstruction->isNOP()){
                        //decode and NOP get no entry in RS only one entry in ROB.
                        instruction_queue.pop_front();
                        int robID = rob.push(robEntry);
                        rob.update(robID, 0, cycle, ROB_COMMIT);
                }else if(!reservationStations.isFull()){
                        instruction_queue.pop_front();
                        RSEntry* reservationStationEntry 
                                = decodeUtility.decodeInstruction(nextInstruction, cycle);
                        reservationStations.addStation(reservationStationEntry);
                        int robID = rob.push(robEntry);
                        robToRS[robID]=reservationStationEntry;
                        reservationStationEntry->updateROBId(robID);
                }
        }else{
                cout<<"Instruction Queue empty or rs full or rob full or no instruction for this cycle."<<endl;
        }
        cout<<"Decode and Issue Close " <<cycle<<endl;
}
void Pipeline::execute(int cycle){
        cout<<"Execute Open:"<<cycle<<endl;
        reservationStations.updateStations(CDB);
        CDB.clear();
        vector<RSEntry*> toBeExecuted = reservationStations.checkPendingReservationStations(cycle);
        //execute instructions in toBeExecuted.
        for(int i=0;i<toBeExecuted.size();i++){
                RSEntry* rs = toBeExecuted.at(i);
                aluUnit.execute(rs,&rob,memory_map);
                if(rs->getRemainingCycles() == 0){
                        //execute then check if complete to move it to next stage.
                        rs->print();
                        executedInstruction.push_back(make_pair(rs,cycle));
                }
        }
        cout<<"Execute End:"<<cycle<<endl;
}
void Pipeline::writeResult(int cycle){
        cout<<"WriteResult Open"<<cycle<<endl;
        //write result to ROB and waiting RS through CDB
        vector<pair<RSEntry*,int> >::iterator executedInstructionIter = executedInstruction.begin();
        vector<pair<RSEntry*,int> > remainingInstructions;
        for(;executedInstructionIter!=executedInstruction.end();++executedInstructionIter){
                int instructionCycle = (*(executedInstructionIter)).second;
                RSEntry* completedStation =  (*(executedInstructionIter)).first;
                Instruction* instruction = completedStation->instruction;
                int robID = completedStation->getROBId();
                if(instructionCycle == cycle){
                        if(instruction->isStore()){
                                rob.update(robID, completedStation->Vj, instructionCycle, ROB_COMMIT,completedStation->A);
                        }else if(instruction->isJump() || instruction->isBranch()){
                                //enter rob with same cycle so as can be commited in next cycle
                                //hack for skipping this stage.
                                int result = completedStation->result;
                                rob.update(robID, result, instructionCycle, ROB_COMMIT);
                        }else{
                                remainingInstructions.push_back(make_pair(completedStation,instructionCycle));
                        }
                }else if(instructionCycle < cycle){
                        //TODO check for other instructions.
                        int result = completedStation->result;
                        //computed value to ROB and write results to CDB.
                        rob.update(robID, result, cycle, ROB_COMMIT);
                        CDB[robID] = result;
                }else{
                        remainingInstructions.push_back(make_pair(completedStation,instructionCycle));
                }
        }
        executedInstruction.clear();
        executedInstruction.insert(executedInstruction.end(),remainingInstructions.begin(),remainingInstructions.end());
        cout<<"WriteResult Close"<<cycle<<endl;
        //Store, Jump Branch NOP and break skip this stage.
}
bool Pipeline::commit(int cycle){
        cout<<"Commit Open "<<cycle<<endl;
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
                if(robToRS.find(rob.getHeadID()) != robToRS.end()){
                        reservationStations.remove(robToRS[rob.getHeadID()]);
                        robToRS.erase(rob.getHeadID());
                }
                rob.pop();
                if(registerStat.getRegisterReorderEntryID(destination) == rob.getHeadID())
                        registerStat.updateRegister(destination, false, -1);
        }
        cout<<"Commit Close "<<cycle<<endl;
}
