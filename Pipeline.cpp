#include "Pipeline.h"
//TODO make pair and return for each rs.
pair<int,int>* ALUUnit::execute(RSEntry* reservationStation, ROB* rob,map<int,Abstract*>* memory){
        pair<int,int>* robCommit = NULL;
        //construct a vector for updating values.
        Instruction* instruction = reservationStation->instruction;
        if(instruction->isLoad()){
                if(reservationStation->numCycles == 2 
                                && rob->allAddressComputedBefore(reservationStation->getROBId())){
                        int memoryAddress = instruction->execute(reservationStation->Vj, reservationStation->Vk);
                        reservationStation->A = memoryAddress;
                        //robCommit = new pair<int,int>(memoryAddress, reservationStation->getROBId());
                        reservationStation->numCycles =  reservationStation->numCycles -1;
                }else if(reservationStation->numCycles == 1
                                && rob->noPendingStore(reservationStation->getROBId())){
                        reservationStation->result = memory->find(reservationStation->A)->second->getValue();
                        reservationStation->numCycles--;
                }
        }else if(instruction->isStore()){
                if(rob->allAddressComputedBefore(reservationStation->getROBId())){
                        if(reservationStation->isReady()){
                                int memoryAddress = instruction->execute(reservationStation->Vj, reservationStation->Vk);
                                robCommit = new pair<int,int>(memoryAddress, reservationStation->getROBId());
                                reservationStation->A = memoryAddress;
                                reservationStation->result = reservationStation->Vk;
                                reservationStation->numCycles--;
                        }else if(reservationStation->isStoreReady()){
                                int memoryAddress = instruction->execute(reservationStation->Vj, reservationStation->Vk);
                                robCommit = new pair<int,int>(memoryAddress, reservationStation->getROBId());
                                reservationStation->A = memoryAddress;
                        }
                }
        }else if(instruction->isJump()){
                reservationStation->result = ((J_Instruction*)instruction)->execute();
                debug("Jump result %d", reservationStation->result);
                reservationStation->numCycles = reservationStation->numCycles -1;
        }else{
                reservationStation->result = instruction->execute(reservationStation->Vj,reservationStation->Vk);
                reservationStation->numCycles = reservationStation->numCycles -1;
        }        
        //based on results output update value in reservation station.
        return robCommit;
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
        debug("printPipeline");
        vector<string> pipelineInstructions;
        
        debug("printing IQ");
        pipelineInstructions.push_back("IQ:");
        deque<pair<int,Instruction*> >::iterator instructionQueueIter = instruction_queue.begin();
        for(;instructionQueueIter!=instruction_queue.end();++instructionQueueIter){
                pipelineInstructions.push_back("[" + instructionQueueIter->second->print(false)+ "]");
        }

        debug("Printing RS ");
        pipelineInstructions.push_back("RS:");
        vector<string> reservationsStationInstructions = reservationStations.print();
        pipelineInstructions.insert(pipelineInstructions.end(),
                        reservationsStationInstructions.begin(),
                        reservationsStationInstructions.end());
        
        debug("Printing ROB");
        pipelineInstructions.push_back("ROB:");
        vector<string> ROBInstructions = rob.print();
        pipelineInstructions.insert(pipelineInstructions.end(),
                        ROBInstructions.begin(),
                        ROBInstructions.end());
        
        debug("Printing BTB");
        pipelineInstructions.push_back("BTB:");
        vector<string> BTBTable = btb.print();
        pipelineInstructions.insert(pipelineInstructions.end(), BTBTable.begin(), BTBTable.end());

        debug("Printing Register Values");
        pipelineInstructions.push_back("Registers:");
        vector<string> registerValues = registerFile.print();
        pipelineInstructions.insert(pipelineInstructions.end(), registerValues.begin(), registerValues.end());
        
        debug("Printing memory Values");
        pipelineInstructions.push_back("Data Segment:");
        stringstream ss;
        ss<<716<<":    ";
        for(int i=0;i<10;i++){
                ss<<memory_map->find(716 + i*4)->second->getValue();
                if(i!=9)
                        ss<<"    ";
        }
        pipelineInstructions.push_back(ss.str());
        
        return pipelineInstructions;
}
void Pipeline::resetPipeline(int robID, int PC){
       //flush mapping for ROBID to RS as well
        map<int,RSEntry*>::iterator iter = robToRS.begin();
        map<int,RSEntry*> robToRSTempMap;
        for(;iter!=robToRS.end();++iter){
               if(iter->first <= robID)
                      robToRSTempMap[iter->first] = iter->second; 
        }
        
        reservationStations.reset(robToRS[robID]);
        rob.reset(robID);
       
        //finally swap
        robToRS.swap(robToRSTempMap);        
        
        map<int,int>::iterator cdbIter = CDB.begin();
        map<int, int> tempCDB;
        for(;cdbIter!=CDB.end();++cdbIter){
                if(cdbIter->first <= robID)
                        tempCDB[cdbIter->first] = cdbIter->second;
        }
        CDB.swap(tempCDB);

        instruction_queue.clear();
        this->PC = PC;
}
void Pipeline::instructionFetch(int cycle){
        if(PC < 716){
                debug("Instruction Fetch open: %d PC : %d", cycle, PC);
                Instruction* nextInstruction = (Instruction*)memory_map->find(PC)->second;
                if(nextInstruction!=NULL){
                        if(nextInstruction->isBranch() || nextInstruction->isJump()){
                                PC = btb.predict(PC);
                                predictedAddress[nextInstruction] = PC;
                        }else
                                PC = PC + 4;
                        instruction_queue.push_back(make_pair(cycle,(Instruction*)nextInstruction));
                }else{
                        debug("No more instructions in memory");
                }
        }
        debug("Instruction Fetch close: %d",cycle);
}
void Pipeline::decodeAndIssue(int cycle){
        debug("Decode and Issue Open");
        if(!instruction_queue.empty() && !rob.isFull()
                        && instruction_queue.front().first < cycle) {
                Instruction* nextInstruction = instruction_queue.front().second;
                if(nextInstruction->isBreak() || nextInstruction->isNOP()){
                        //decode and NOP get no entry in RS only one entry in ROB.
                        decodeUtility.decodeNOPAndBreak(nextInstruction, cycle);
                        instruction_queue.pop_front();
                }else if(!reservationStations.isFull()){
                        instruction_queue.pop_front();
                        RSEntry* reservationStationEntry 
                                = decodeUtility.decodeInstruction(nextInstruction, cycle);
                        reservationStations.addStation(reservationStationEntry);
                        robToRS[reservationStationEntry->getROBId()]=reservationStationEntry;
                }else{
                        debug("Stall");
                }
        }else{
                debug("Instruction Queue empty or rs full or rob full or no instruction for this cycle.");
        }
        debug("Decode and Issue Close ");
}
void Pipeline::execute(int cycle){
        debug("Execute Open: %d", cycle);
        //reservationStations.updateStations(CDB);
        reservationStations.updateStations(&rob);
        CDB.clear();
        vector<RSEntry*> toBeExecuted = reservationStations.checkPendingReservationStations(cycle);
        vector<pair<int,int>* > robCommits;
        //execute instructions in toBeExecuted.
        for(int i=0;i<toBeExecuted.size();i++){
                RSEntry* rs = toBeExecuted.at(i);
                pair<int,int>* robCommit = aluUnit.execute(rs,&rob,memory_map);
                if(robCommit != NULL)
                        robCommits.push_back(robCommit);
                if(rs->getRemainingCycles() == 0){
                        //execute then check if complete to move it to next stage.
                        if(rs->instruction->isBranch() || rs->instruction->isJump()){
                                int evaluatedAddress = rs->result;
                                if(predictedAddress[rs->instruction] != evaluatedAddress){
                                        debug("branch misprediction");
                                        resetPipeline(rs->robID,evaluatedAddress);
                                        btb.update(rs->instruction->getMemory(),evaluatedAddress,rs->instruction->isBranchTaken);
                                }
                        }
                        executedInstruction.push_back(make_pair(rs,cycle));
                }
        }
        for(int i=0;i<robCommits.size();i++)
                rob.setDestination(robCommits.at(i)->first, robCommits.at(i)->second);
        debug("Execute End: %d", cycle);
}
void Pipeline::writeResult(int cycle){
        debug("WriteResult Open %d", cycle);
        debug("ROB Size %d",rob.size());

        //write result to ROB and waiting RS through CDB
        vector<pair<RSEntry*,int> >::iterator executedInstructionIter = executedInstruction.begin();
        vector<pair<RSEntry*,int> > remainingInstructions;
        for(;executedInstructionIter!=executedInstruction.end();++executedInstructionIter){
                int instructionCycle = (*(executedInstructionIter)).second;
                RSEntry* completedStation =  (*(executedInstructionIter)).first;
                Instruction* instruction = completedStation->instruction;
                int robID = completedStation->getROBId();
                if(robToRS.find(robID)==robToRS.end())
                        continue;
                if(instruction->isStore()){
                        //store Vj at A.
                        debug("Store instruction %s value to save %d, at %d",
                                        instruction->print(false).c_str(),completedStation->Vj,completedStation->A);
                        rob.update(robID, completedStation->Vk, instructionCycle, ROB_COMMIT, completedStation->A);
                }else if(instruction->isJump() || instruction->isBranch()){
                        //enter rob with same cycle so as can be commited in next cycle
                        //hack for skipping this stage.
                        int result = completedStation->result;
                        rob.update(robID, result, instructionCycle, ROB_COMMIT);
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
        debug("ROB Size %d",rob.size());
        debug("WriteResult Close");
        //Store, Jump Branch NOP and break skip this stage.
}
bool Pipeline::commit(int cycle){
        bool executeNextCycle = true;
        debug("Commit Open ");
        debug("ROB Size %d",rob.size());
        if(!rob.isEmpty() 
                        && rob.peek()->getState() == ROB_COMMIT 
                        && rob.peek()->getCycle() < cycle){
                ROBEntry* robHead = rob.peek();
                int destination = robHead->getDestination();
                if(robHead->getInstruction()->isBranch() || robHead->getInstruction()->isJump()){
                        RSEntry* rs = robToRS[rob.getHeadID()];
                        if(predictedAddress[rs->instruction] != rs->result){
                                //TODO misprediction
                                ;
                                //resetPipeline(rob.getHeadID(), rs->result);
                        } 
                        predictedAddress.erase(rs->instruction);
                }else if(robHead->getInstruction()->isStore()){
                        (*memory_map)[destination] = new Abstract(robHead->getValue(),destination);
                }else if(robHead->getInstruction()->isBreak())
                        executeNextCycle = false;
                else{
                        debug("Updating register value for R%d whose %d and head is %d value %d",
                                        destination, registerStat.getRegisterReorderEntryID(destination), 
                                        rob.getHeadID(), robHead->getValue());
                        registerFile.setRegisterValue(destination, robHead->getValue());  
                }
                if(registerStat.getRegisterReorderEntryID(destination) == rob.getHeadID()){
                        debug("Updating register R%d", destination);
                        registerStat.updateRegister(destination, false, -1);
                }
                if(robToRS.find(rob.getHeadID()) != robToRS.end()){
                        reservationStations.remove(robToRS[rob.getHeadID()]);
                        robToRS.erase(rob.getHeadID());
                }
                rob.pop();

        }
        debug("ROB size %d", rob.size());
        debug("Commit Close ");
        return executeNextCycle;
}
