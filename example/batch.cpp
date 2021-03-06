#include <cstdio>
#include "sched_test.h"

/*
 * sched_test option...
 * 
 */

int main(int argc, char **argv)
{
    int ncpu = 1;
    int nproc;
    int timeSlice = 100000;
    int interruptRate = 5;

    if(argc < 2)
    {
        printf("Usage: %s nproc [interruptRate=%d]\n", argv[0], interruptRate);
        return 0;
    }

    nproc = atoi(argv[1]);

    if(argc == 3)
        interruptRate = atoi(argv[2]);

    SchedTest st(ncpu);
    for(int iproc = 0; iproc < nproc; iproc++)
        st.createProcess(0, interruptRate);

    for(int iproc = 0; iproc < nproc; iproc++)
    {
        ProcessState ps = READY;
        while(ps != EXIT)
        {
            st.setFirst(iproc, timeSlice);
            auto ret = st.commit();

            int proc = ret.first;
            ps = ret.second;
            printf("ps : %d\n", (int)ps);
        }
        printf("fairness : %.2f%%\n", st.getFairness() * 100);
        printf("runtime : %d\n", st.getRuntime());
    }

    printf("runtime : %d\n", st.getRuntime());
    return 0;
}