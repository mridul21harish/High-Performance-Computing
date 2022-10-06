#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int myid, np;
    double startwtime, endwtime, totalTime;
    int namelen;
    int n = 100000;
    double a[n + 50], b[n + 50], c[n + 50];
    int i, j;
    int s, s0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    startwtime = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    //fprintf(stderr, "Process %d on %s\n", myid, processor_name);
    fflush(stderr);
    // Vector 1 Reading
    for (i = 0; i < n; i++)
        a[i] = i;
    // Vector 2 Reading
    for (i = 0; i < n; i++)
        b[i] = i;
    if (myid == 0)
    {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        s = (int)floor(n / np);
        s0 = n % np;
        int a_recv[s];
        int b_recv[s];
        long int c_recv[s];
        if (s0 != 0)
        {
            s = s + 1;
            for (i = 0; i < ((s * np) - n); i++)
            {
                a[n + i] = 1;
                b[n + i] = 1;
            }
        }
        MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(a, s, MPI_INT, a_recv, s, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(b, s, MPI_INT, b_recv, s, MPI_INT, 0, MPI_COMM_WORLD);
        for (i = 0; i < s; i++)
        {
            c_recv[i] = a_recv[i] + b_recv[i];
        }
        MPI_Gather(c_recv, s, MPI_LONG, c, s, MPI_LONG, 0, MPI_COMM_WORLD);

        //for (i = 0; i < n; i++)
            //printf("%d %d %d %ld\n", i, a[i], b[i], c[i]);
        endwtime = MPI_Wtime();
        totalTime = endwtime - startwtime;
        printf("%f\n", totalTime);
    }
    else
    {
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        double a_recv[s], b_recv[s], c_recv[s];
        MPI_Scatter(a, s, MPI_INT, a_recv, s, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(b, s, MPI_INT, b_recv, s, MPI_INT, 0, MPI_COMM_WORLD);
        for (i = 0; i < s; i++)
        {
            c_recv[i] = a_recv[i] + b_recv[i];
        }
        MPI_Gather(c_recv, s, MPI_LONG, c, s, MPI_LONG, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
}