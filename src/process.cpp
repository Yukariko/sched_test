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
    deferredTime = proc.deferredTime;
    ps = proc.ps;
    interruptRate = proc.interruptRate;
}

/*
 * 프로세스의 실행 부분.
 * 시작과 끝을 알리는 메세지, 중간중간 프로세스 작업에 관한 메세지를 전달함
 * 각 작업에는 소요되는 시간이 명세되어야 할 것으로 보임
 */

Process::Process(int id, int cpu, int interruptRate)
{
    this->id = id;
    this->cpu = cpu;
    this->interruptRate = interruptRate;
    necessaryTime = 1000;
}

int Process::processJob(int timeSlice)
{
    int spentTime = std::min(std::min(jobTime, timeSlice),
                            necessaryTime - runtime - deferredTime);
    jobTime -= spentTime;
    runtime += spentTime;
    updatedClock += spentTime;

    if (ps == INTERRUPT)
    {
        interruptTime = std::max(0, interruptTime - spentTime);
    }
    return spentTime;
}

ProcessState Process::updateState()
{
    if (runtime + deferredTime >= necessaryTime)
        return ps = EXIT;
    if (interruptTime > 0)
        return ps = INTERRUPT;
    return ps = READY;
}

void Process::updateClock(int clock)
{
    if (interruptTime > 0)
    {
        int diffTime = clock - updatedClock;
        deferredTime += std::min(interruptTime, diffTime);
        jobTime = std::max(0, jobTime - std::min(interruptTime, diffTime));
        interruptTime = std::max(0, interruptTime - diffTime);
    }
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

        if (jobTime == 0)
        {
            chooseJob();
            if (ps != READY)
                return ps;
        }
    }
    return ps;
}

void Process::chooseJob()
{
    if (runtime + deferredTime >= necessaryTime)
    {
        ps = EXIT;
        return;
    }

    int r = rand() % 100;
    if (r < interruptRate)
    {
        interruptTime = 100;
        jobTime = 100;
        ps = INTERRUPT;
    }
    else
    {
        interruptTime = 0;
        jobTime = 100;
        ps = READY;
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

int Process::getModTime()
{
    return modTime;
}