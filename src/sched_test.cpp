#include <algorithm>
#include <climits>
#include "sched_test.h"

SchedTest::SchedTest(int ncpu)
{
    this->ncpu = ncpu;
    cpus.resize(ncpu);
}

int SchedTest::createProcess(int cpu)
{
    if(ncpu <= cpu)
        return -1;

    procs.push_back(Process(counter, cpu));
    cpus[cpu].insert(&procs.back());
    
    return counter++;
}

void SchedTest::changeCpu(int id, int cpu)
{
    int curCpu = procs[id].getCpu();
    if (curCpu == cpu)
        return;

    cpus[curCpu].remove(&procs[id]);
    cpus[cpu].insert(&procs[id]);
    nChange++;
    procs[id].setCpu(cpu);
}

void SchedTest::setFirst(int id, int timeSlice)
{
    int cpu = procs[id].getCpu();
    cpus[cpu].setFirst(&procs[id], timeSlice);
    timeSlices[{runtime + timeSlice, cpu}] = id;
}

std::pair<int, ProcessState> SchedTest::commit()
{
    int proc = timeSlices.begin()->second;
    int minTimeSliceCpu = timeSlices.begin()->first.second;
    int minTimeSlice = timeSlices.begin()->first.first - runtime;

    ProcessState ret = cpus[minTimeSliceCpu].commit(runtime);
    nContextSwitch++;
    runtime += minTimeSlice - procs[proc].getModTime();
    timeSlices.erase(timeSlices.begin());
    return {proc, ret};
}

int SchedTest::getModTime(int id)
{
    return procs[id].getModTime();
}

int SchedTest::getRuntime()
{
    return runtime;
}

double SchedTest::getFairness()
{
    if (!counter)
        return 0.;

    int sumClock = 0;
    for(int iproc = 0; iproc < counter; iproc++)
    {
        int clock = procs[iproc].getRuntime();
        sumClock += clock;
    }

    double meanClock = sumClock / (double)counter;
    double fairness = 0;

    for(int iproc = 0; iproc < counter; iproc++)
    {
        int clock = procs[iproc].getRuntime();
        fairness = std::max(fairness, std::abs(clock - meanClock));
    }
    
    fairness = fairness / sumClock;
    fairness = 1. - fairness;
    return fairness;
}