#include <cstdio>
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

    if(argc < 2)
    {
        printf("Usage: %s nproc [timeslice=%d]\n", argv[0], timeSlice);
        return 0;
    }

    nproc = atoi(argv[1]);

    if(argc == 3)
        timeSlice = atoi(argv[2]);

    SchedTest st(ncpu);
    std::list<int> procs;
    for(int iproc = 0; iproc < nproc; iproc++)
        procs.push_back(st.createProcess(0));

    while(!procs.empty())
    {
        int select = -1;
        for(auto it = procs.begin(); it != procs.end(); it++)
        {
            ProcessState ps = st.getState(*it);
            if(ps == READY)
                select = 1;
            while(ps == READY)
            {
                st.setFirst(*it, timeSlice);
                auto ret = st.commit();

                int proc = ret.first;
                ps = ret.second;
                if(ps == EXIT)
                {
                    procs.erase(it--);
                }
                printf("ps : %d\n", ps);
            }

            printf("fairness : %.2f%%\n", st.getFairness() * 100);
            printf("runtime : %d\n", st.getRuntime());
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
    }


    printf("runtime : %d\n", st.getRuntime());
    return 0;
}