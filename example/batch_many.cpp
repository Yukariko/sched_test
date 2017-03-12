#include <cstdio>
#include <queue>
#include <algorithm>
#include <vector>
#include "sched_test.h"

/*
 * sched_test option...
 * 
 */

int main(int argc, char **argv)
{
    int ncpu;
    int nproc;
    int timeSlice = 100000;

    if(argc < 3)
    {
        printf("Usage: %s ncpu nproc\n", argv[0]);
        return 0;
    }

    ncpu = atoi(argv[1]);
    nproc = atoi(argv[2]);

    if(argc == 4)
        timeSlice = atoi(argv[3]);

    SchedTest st(ncpu);

    std::vector<std::queue<int>> cpus(ncpu);
    for(int iproc = 0; iproc < nproc; iproc++)
        cpus[iproc % ncpu].push(st.createProcess(iproc % ncpu));

    for(int icpu = 0; icpu < ncpu && icpu < nproc; icpu++)
        st.setFirst(icpu, timeSlice);

    int rcpu = std::min(ncpu, nproc);
    while(rcpu > 0)
    {
        ProcessState ps = READY;
        int proc;
        while(ps != EXIT)
        {
            auto ret = st.commit();

            proc = ret.first;
            ps = ret.second;
    
            if(ps != EXIT)
                st.setFirst(proc, timeSlice);

            printf("[%d] ps : %d\n", proc, (int)ps);
        }

        int cpu = st.getCpu(proc);
        cpus[cpu].pop();
        if(!cpus[cpu].empty())
            st.setFirst(cpus[cpu].front(), timeSlice);
        else
            rcpu--;

        printf("fairness : %.2f%%\n", st.getFairness() * 100);
        printf("runtime : %d\n", st.getRuntime());

    }

    printf("runtime : %d\n", st.getRuntime());
    printf("context switch : %d\n", st.getContextSwitch());
    return 0;
}