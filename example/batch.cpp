#include <cstdio>
#include "sched_test.h"

/*
 * sched_test option...
 * 
 */

int main(int argc, char **argv)
{
    int ncpu = 1;
    int nproc = 10;
    int timeSlice = 3000;
    int runtime = 0;

    SchedTest st(ncpu);
    for(int iproc = 0; iproc < nproc; iproc++)
        st.createProcess(0);

    for(int iproc = 0; iproc < nproc; iproc++)
    {
        ProcessState ps = READY;
        while(ps != EXIT)
        {
            st.setFirst(iproc, timeSlice);
            auto ret = st.commit();

            int proc = ret.first;
            ps = ret.second;
            printf("ps : %d\n", ps);
        }
        printf("fairness : %.2f%%\n", st.getFairness() * 100);
        printf("runtime : %d\n", st.getRuntime());
    }

    printf("runtime : %d\n", st.getRuntime());
    return 0;
}