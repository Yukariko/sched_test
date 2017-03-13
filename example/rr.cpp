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
    int ncpu = 1;
    int nproc;
    int timeSlice = 30;
    int interruptRate = 5;

    if(argc < 2)
    {
        printf("Usage: %s nproc [timeslice=%d] [interruptRate=%d]\n", argv[0], timeSlice, interruptRate);
        return 0;
    }

    nproc = atoi(argv[1]);

    if(argc >= 3)
        timeSlice = atoi(argv[2]);

    if(argc >= 4)
        interruptRate = atoi(argv[3]);

    SchedTest st(ncpu);
    std::list<int> procs;
    for(int iproc = 0; iproc < nproc; iproc++)
        procs.push_back(st.createProcess(0, interruptRate));

    while(!procs.empty())
    {
        int select = -1;
        for(auto it = procs.begin(); it != procs.end(); it++)
        {
            if (st.getState(*it) == INTERRUPT)
                continue;
            select = *it;
            st.setFirst(*it, timeSlice);
            auto ret = st.commit();

            if(ret.second == EXIT)
            {
                procs.erase(it--);
            }
        }

        if (select == -1)
        {
            st.setFirst(*procs.begin(), timeSlice);
            auto ret = st.commit();

            if(ret.second == EXIT)
            {
                procs.erase(procs.begin());
            }
        }
        printf("fairness : %.2f%%\n", st.getFairness() * 100);
        printf("runtime : %d\n", st.getRuntime());
    }



    printf("runtime : %d\n", st.getRuntime());
    return 0;
}