#ifndef SCHED_TEST_H
#define SCHED_TEST_H

#include <vector>
#include <map>
#include "process.h"
#include "cpu.h"

class SchedTest
{
public:
    SchedTest(int ncpu = 1);
    int createProcess(int cpu = 0, int interruptRate = 5);
    std::pair<int, ProcessState> commit();
    void changeCpu(int id, int cpu = 0);
    void setFirst(int id, int timeSlice);

    int getRuntime();
    ProcessState getState(int id);
    double getFairness();
    int getContextSwitch();
    int getCpu(int proc);

private:
    std::vector<Cpu> cpus;
    std::vector<Process> procs;
    std::map<std::pair<int, int>, std::pair<int, int>> timeSlices;
    int ncpu;
    int counter = 0;
    int runtime = 0;
    int nChange = 0;
    int nContextSwitch = 0;
};


#endif