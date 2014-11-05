#include "PipelineUnits.h"
class Pipeline{
        private:
                //10 integer ALU RS entries
                RSEntry reservationStation[10];
                int reservationStationIndex;
                //6 Entries
                ROBEntry ROB[6];
                int ROBIndex;
        public:
                void instructionFetch();
                void decodeAndIssue();
                void execute();
                void writeResult();
                void commit();
};
