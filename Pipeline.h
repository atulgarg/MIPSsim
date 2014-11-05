#include "PipelineUnits.h"
class Pipeline{
        public:
                void instructionFetch();
                void decodeAndIssue();
                void execute();
                void writeResult();
                void commit();
};
