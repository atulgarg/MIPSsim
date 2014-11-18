#include "Instructions.h"
//Constants used for reorder Buffer
#define ROB_COMMIT 0
#define ROB_WRITE_RESULT 1
#define ROB_EXECUTE 2
enum ROBState {ROB_COMMIT, ROB_WRITE_RESULT, ROB_EXECUTE};

//Constants used for BTB.
#define PREDICTION_TAKEN 1
#define PREDICTION_NOT_TAKEN 0
enum Prediction {PREDICTION_TAKEN, PREDICTION_NOT_TAKEN};
class RSEntry{
        //Opcode
        bool busy;
        int Vj;
        int Vk;
        RSEntry* Qj;
        RSEntry* Qk;
        int A;
        int reorderEntryID; //destination.
        public:
        RSEntry(bool busy,int Vj,int Vk,RSEntry* Qj,RSEntry* Qk,int A);
        bool isBusy();
        void setBusy(bool busy);
        void updateReorderID(int reorderEntryID);
};
class ReservationStations{
        RSEntry* reservationStations[10];
        int currently_used;
        int max_stations;
        int findNextEmpty(); 
        public:
        ReservationStations(int max_stations);
        RSEntry* getReservationStation();
};
class ROBEntry{
        bool busy;
        Instructions* instruction;
        ROBState state;
        int destination;    //differentiate between memory and register??
        double value;
};
class ROB{
        ROBEntry** rob;
        int front,rear;
        int max_entries;
        public:
        ROB(int max_entries);
        void addROBEntry();
        bool isFull();
}
class RegisterStatEntry{
        bool busy;
        //id for reorder entry
        int reorderEntryID;
        public:
        RegisterStatEntry();
        RegisterStatEntry(bool busy, int reorderEntryID);
        bool isBusy();
        void update(bool busy,int reorderEntryID);
};
class BTBEntry{
        int instructionAddress;
        int branchTargetAddress;
        Prediction prediction;
        //LRU replacement policy;
};
class RegisterFile{
        int num_registers;
        RegisterStatEntry* registers;
        public:
        RegisterFile();
        bool isRegisterBusy(int reg);
        void updateRegister(int reg,bool busy, int reorderEntryID);
};
