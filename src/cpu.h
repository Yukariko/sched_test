#ifndef CPU_H
#define CPU_H

#include <vector>
#include <map>
#include "process.h"

class Cpu
{
public:
    Cpu(){}
    void insert(Process* proc);
    void remove(Process* proc);
    void setFirst(Process* proc, int timeSlice);
    ProcessState commit();
    int getRuntime();
    int getTimeSlice();

private:
    std::map<int, Process*> procs;
    Process* firstNode = nullptr;
    int clock = 0;
    int interruptTime = 0;
    int timeSlice = -1;
};

#endif // CPU_H
