#include <cstdlib>
#include <algorithm>
#include "process.h"

Process::Process(const Process& proc)
{
    cpu = proc.cpu;
    id = proc.id;
    necessaryTime = proc.necessaryTime;
    runtime = proc.runtime;
    jobTime = proc.jobTime;
    interruptTime = proc.interruptTime;
    modTime = proc.modTime;
    ps = proc.ps;
}

/*
 * 프로세스의 실행 부분.
 * 시작과 끝을 알리는 메세지, 중간중간 프로세스 작업에 관한 메세지를 전달함
 * 각 작업에는 소요되는 시간이 명세되어야 할 것으로 보임
 */

Process::Process(int id, int cpu)
{
    this->id = id;
    this->cpu = cpu;
    necessaryTime = 1000;
    chooseJob();
}

int Process::processJob(int timeSlice)
{
    int spentTime = std::min(std::min(jobTime, timeSlice),
                            necessaryTime - runtime);
    jobTime -= spentTime;
    if (interruptTime == 0)
        runtime += spentTime;

    return spentTime;
}

ProcessState Process::updateState()
{
    if (runtime >= necessaryTime)
        return ps = EXIT;
    if (interruptTime > 0)
        return ps = INTERRUPT;
    return ps = READY;
}

void Process::updateClock(int clock)
{
    int diffTime = clock - updatedClock;
    if (interruptTime > 0)
        interruptTime = std::max(0, interruptTime - diffTime);
    updatedClock = clock;
}

ProcessState Process::run(int timeSlice, int clock)
{
    updateClock(clock);
    modTime = timeSlice;
    while(modTime > 0 && ps != EXIT)
    {
        int spentTime = processJob(modTime);        
        modTime -= spentTime;
        updatedClock += spentTime;

        if (jobTime == 0)
        {
            chooseJob();
            updateState();
            if (ps != READY)
                return ps;
        }
    }
    return ps;
}

void Process::chooseJob()
{
    int r = rand() % 100;
    if (r < 5)
    {
        interruptTime = 100;
        jobTime = 100;
    }
    else
    {
        interruptTime = 0;
        jobTime = 100;        
    }
}

int Process::getInterruptTime()
{
    return interruptTime;
}

void Process::setCpu(int cpu)
{
    this->cpu = cpu;
}

int Process::getCpu()
{
    return cpu;
}

int Process::getModTime()
{
    return modTime;
}

int Process::getClock()
{
    return updatedClock;
}

int Process::getRuntime()
{
    return runtime;
}

ProcessState Process::getState(int clock)
{
    updateClock(clock);
    updateState();
    return ps;
}

int Process::getId()
{
    return id;
}