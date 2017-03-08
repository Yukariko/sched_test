#ifndef PROCESS_H
#define PROCESS_H

enum ProcessState {READY, INTERRUPT, EXIT};

class Process
{
public:
    Process() {}
    Process(const Process& proc);
    Process(int id, int cpu);
    ProcessState run(int timeSlice);
    int processJob(int timeSlice);

    void setCpu(int cpu);
    int getId();
    int getCpu();
    int getInterruptTime();
    int getModTime();
    int getClock();
    int getRuntime();
    ProcessState getProcessState();

private:
    void chooseJob();

    int cpu = -1;
    int id;
    int necessaryTime;
    int runtime = 0;
    int jobTime;
    int interruptTime = 0;
    int modTime = 0;
    int updatedClock = 0;
    ProcessState ps;
};

#endif