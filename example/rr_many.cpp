#include <cstdio>
#include <algorithm>
#include <list>
#include "sched_test.h"

/*
 * sched_test option...
 * 
 */

int main(int argc, char **argv)
{
    int ncpu;
    int nproc;
    int timeSlice = 30;
    int interruptRate = 5;

    if(argc < 3)
    {
        printf("Usage: %s ncpu nproc [timeslice=%d] [interruptRate=%d]\n", argv[0], timeSlice, interruptRate);
        return 0;
    }

    ncpu = atoi(argv[1]);
    nproc = atoi(argv[2]);

    if(argc >= 4)
        timeSlice = atoi(argv[3]);

    if(argc >= 5)
        interruptRate = atoi(argv[4]);

    SchedTest st(ncpu);

    std::vector<std::list<int>> procs(ncpu);
    for(int iproc = 0; iproc < nproc; iproc++)
        procs[iproc % ncpu].push_back(st.createProcess(iproc % ncpu, interruptRate));

    for(int icpu = 0; icpu < ncpu && icpu < nproc; icpu++)
    {
        int proc = procs[icpu].front();
        st.setFirst(proc, timeSlice);
        procs[icpu].pop_front();
        procs[icpu].push_back(proc);
    }

    int rcpu = std::min(nproc, ncpu);
    while(rcpu > 0)
    {
        ProcessState ps = READY;
        int proc;
        auto ret = st.commit();

        proc = ret.first;
        ps = ret.second;
        printf("[%d] ps : %d\n", proc, (int)ps);

        int cpu = st.getCpu(proc);
        bool select = false;
        for(auto it = procs[cpu].begin(); it != procs[cpu].end(); it++)
        {
            int proc = *it;
            ProcessState ps = st.getState(proc);
            if(ps == READY)
            {
                select = true;
                st.setFirst(proc, timeSlice);
                procs[cpu].erase(it);
                procs[cpu].push_back(proc);
                break;
            }
            else if (ps == EXIT)
            {
                procs[cpu].erase(it--);
            }
        }

        if (select == false)
        {
            for(auto it = procs[cpu].begin(); it != procs[cpu].end(); it++)
            {
                int proc = *it;
                ProcessState ps = st.getState(proc);
                if(ps != EXIT)
                {
                    select = true;
                    st.setFirst(proc, timeSlice);
                    procs[cpu].erase(it);
                    procs[cpu].push_back(proc);
                    break;
                }
                else
                {
                    procs[cpu].erase(it--);
                }
            }
        }

        if(procs[cpu].empty())
            rcpu--;
 
        printf("fairness : %.2f%%\n", st.getFairness() * 100);
        printf("runtime : %d\n", st.getRuntime());
    }


    printf("runtime : %d\n", st.getRuntime());
    printf("context switch : %d\n", st.getContextSwitch());
    return 0;
}