#ifndef _RESERVATION_STATION_H_
#define _RESERVATION_STATION_H_
#include<vector>
#include<cstdlib>
#include<map>
#include<string>
#include<assert.h>
#include "Instructions.h"
using namespace std;
class RSEntry{
        friend class ALUUnit;
        friend class Pipeline;
        Instruction* instruction;
        bool busy;
        int A;
        int cycle, numCycles;          //number of cycles required for execution.
        int result, robID;
        public:
        int ROBId_Qj, ROBId_Qk, Vj, Vk;
        RSEntry(Instruction* instruction,bool busy,int Vj,int Vk,int ROBId_Qj,int ROBId_Qk,int A,int cycle, int numCycles);
        bool isBusy();
        void setBusy(bool busy);
        void updateROBId(int robID);
        int getROBId();
        int getCycle();
        bool isReady();
        int execute();
        int getRemainingCycles();
        std::string print();
};
class ReservationStations{
        std::vector<RSEntry*> reservationStations;
        int currently_used;
        int max_stations;
        public:
        ReservationStations(int max_stations);
        void reset(int robID);
        bool isFull();
        bool addStation(RSEntry* reservationStation);
        void updateStations(std::map<int,int> CDB);
        std::vector<RSEntry*> checkPendingReservationStations(int cycle);
        void remove(RSEntry* reservationStation);
        std::vector<std::string> print();
};
#endif
