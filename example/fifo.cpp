#include <cstdio>
#include "sched_test.h"

/*
 * sched_test option...
 * 
 */

int main(int argc, char **argv)
{
    int ncpu = 1;
    int nproc = 30;
    int timeSlice = 30;
    int runtime = 0;
    int np = 30;

    SchedTest st(ncpu);
    for(int iproc = 0; iproc < nproc; iproc++)
        st.createProcess(0);

    for(int iproc = 0; iproc < nproc; iproc++)
    {
        ProcessState ps = READY;
        while(ps != EXIT)
        {
            st.setCpu(iproc, 0);
            ps = st.commit(0, timeSlice);
        }
    }

    printf("runtime : %d\n", st.getRuntime());
    return 0;
}