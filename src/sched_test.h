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
    int createProcess(int cpu = 0);
    std::pair<int, ProcessState> commit();
    void changeCpu(int id, int cpu = 0);
    void setFirst(int id, int timeSlice);

    int getModTime(int id);
    int getRuntime();
    ProcessState getState(int id);
    double getFairness();

private:
    std::vector<Cpu> cpus;
    std::vector<Process> procs;
    std::map<std::pair<int, int>, int> timeSlices;
    int ncpu;
    int counter = 0;
    int runtime = 0;
    int nChange = 0;
    int nContextSwitch = 0;
};


#endif