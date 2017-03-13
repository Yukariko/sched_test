#ifndef PROCESS_H
#define PROCESS_H

enum ProcessState {READY, INTERRUPT, EXIT};

class Process
{
public:
    Process() {}
    Process(const Process& proc);
    Process(int id, int cpu, int interruptRate = 5);
    ProcessState run(int timeSlice, int clock);
    int processJob(int timeSlice);

    void setCpu(int cpu);
    void updateClock(int clock);
    ProcessState updateState();

    int getId();
    int getCpu();
    int getInterruptTime();
    int getModTime();
    int getClock();
    int getRuntime();
    ProcessState getState(int clock);

private:
    void chooseJob();

    int cpu;
    int id;
    int necessaryTime;
    int runtime = 0;
    int jobTime = 0;
    int interruptTime = 0;
    int updatedClock = 0;
    int modTime;
    int deferredTime = 0;
    int interruptRate;
    ProcessState ps;
};

#endif