#include "Pipeline.h"
class Assembler{
        Pipeline pipeline;
        map<int,Abstract*>* memory_map;
        int program_counter; 
        public:
        Assembler(map<int,Abstract*>* memory_map,int program_counter);
};
