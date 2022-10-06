#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define n 100000
double a[n], b[n];
double c[n] = {0};
int main(int argc, char *argv[])
{
    int myid, np, elements_per_process, n_elements_recieved;
    MPI_Status status;
    double startwtime, endwtime, totalTime;
    MPI_Init(&argc, &argv);
    startwtime = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);


    if (myid == 0)
    {
        for (int i = 0; i < n; i += 1)
        {
            a[i] = (i + 1);
            b[i] = (i + 1);
        }
        int idx, i;
        if(np == 1)
            elements_per_process = n;
        else
            elements_per_process = n / (np - 1);
        if (np > 1)
        {
            for (i = 1; i < np - 1; i++)
            {
                idx = (i - 1) * elements_per_process;
                MPI_Send(&elements_per_process,1, MPI_DOUBLE, i, 0,MPI_COMM_WORLD);
                MPI_Send(&a[idx],elements_per_process,MPI_DOUBLE, i, 0,MPI_COMM_WORLD);
                MPI_Send(&b[idx],elements_per_process,MPI_DOUBLE, i, 0,MPI_COMM_WORLD);
            }
            idx = (i - 1) * elements_per_process;
            int elements_left = n - idx;
            MPI_Send(&elements_left,1, MPI_DOUBLE,i, 0,MPI_COMM_WORLD);
            MPI_Send(&a[idx],elements_left,MPI_DOUBLE, i, 0,MPI_COMM_WORLD);
            MPI_Send(&b[idx],elements_left,MPI_DOUBLE, i, 0,MPI_COMM_WORLD);
        }
        for (i = 1; i < np; i++)
        {
            int n_elements_recieved;
            idx = (i - 1) * elements_per_process;
            MPI_Recv(&n_elements_recieved,1, MPI_DOUBLE, i, 0,MPI_COMM_WORLD,&status);
            MPI_Recv(&c[idx], n_elements_recieved,MPI_DOUBLE, i, 0,MPI_COMM_WORLD,&status);
            int sender = status.MPI_SOURCE;
        }
        endwtime = MPI_Wtime();
        totalTime = endwtime - startwtime;
        printf("%f\n", totalTime);
    }

    else
    {
        MPI_Recv(&n_elements_recieved,1, MPI_DOUBLE, 0, 0,MPI_COMM_WORLD,&status);
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        double a_recv[n + 1000], b_recv[n + 1000], c_recv[n + 1000];
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);
        MPI_Recv(&a_recv, n_elements_recieved,MPI_DOUBLE, 0, 0,MPI_COMM_WORLD,&status);
        MPI_Recv(&b_recv, n_elements_recieved,MPI_DOUBLE, 0, 0,MPI_COMM_WORLD,&status);
        for (int i = 0; i < n_elements_recieved; i++)
            c_recv[i] = a_recv[i] * b_recv[i];
        MPI_Send(&n_elements_recieved,1, MPI_DOUBLE,0, 0,MPI_COMM_WORLD);
        MPI_Send(&c_recv, n_elements_recieved, MPI_DOUBLE,0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}