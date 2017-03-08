#include <algorithm>
#include "cpu.h"

void Cpu::insert(Process* proc)
{
    procs[proc->getId()] = proc;
}

void Cpu::remove(Process* proc)
{
    procs.erase(proc->getId());
}

void Cpu::setFirst(Process* proc, int timeSlice)
{
    firstNode = proc;
    this->timeSlice = timeSlice;
}

ProcessState Cpu::commit()
{
    ProcessState ps = firstNode->run(timeSlice);
    int spentTime = 0;

    if (ps == READY)
        spentTime = timeSlice;
    else
        spentTime = timeSlice - firstNode->getModTime();

    clock += spentTime;
    interruptTime = std::max(0, interruptTime - spentTime);

    if (ps == INTERRUPT)
        interruptTime = std::max(interruptTime, firstNode->getInterruptTime());

    firstNode = nullptr;
    return ps;
}

int Cpu::getRuntime()
{
    return clock + interruptTime;
}

int Cpu::getTimeSlice()
{
    return timeSlice;
}