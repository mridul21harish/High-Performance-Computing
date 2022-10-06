#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define n_row 300
#define n_col 300

int main(int argc, char *argv[])
{
    int myid, numprocs;
    int namelen;
    double mat_1[n_row + 50][n_col];
    double mat_2[n_row + 50][n_col];
    long double mat_3[n_row + 50][n_col];
    int i, j;
    int s, s0;
    double totalTime;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);
    //fprintf(stderr, "Process %d on %s\n", myid, processor_name);
    double start_time = MPI_Wtime();

    for (int i = 0; i < n_row; i++)
        for (int j = 0; j < n_col; j++)
        {
            mat_1[i][j] = i + j;
            mat_2[i][j] = i + j;
        }

    if (myid == 0)
    {
        s = (int)floor(n_row / numprocs);
        s0 = n_row % numprocs;
        double mat_1_recv[s][n_col];
        double mat_2_recv[s][n_col];
        long double mat_3_recv[s][n_col];
        if (s0 != 0)
        {
            s = s + 1;
            for (i = 0; i < ((s * numprocs) - n_row); i++)
                for (j = 0; j < n_col; j++)
                {
                    mat_1[n_row + i][j] = 1;
                    mat_2[n_row + i][j] = 1;
                }
        }
        MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(mat_1, s, MPI_DOUBLE, mat_1_recv, s, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mat_2, n_row * n_col, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for (int k = 0; k < n_col; k++)
            for (int i = 0; i < s; i++)
            {
                mat_3_recv[i][k] = 0.0;
                for (int j = 0; j < n_col; j++)
                    mat_3_recv[i][k] = mat_3_recv[i][k] + mat_1_recv[i][j] * mat_2[j][k];
            }
        MPI_Gather(mat_3_recv, s, MPI_LONG_DOUBLE, mat_3, s, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

        // for (int i = 0; i < n_row; i++)
        //     for (int j = 0; j < n_col; j++)
        //         printf("%d %d %f %f %Lf\n", i, j, mat_1[i][j], mat_2[i][j], mat_3[i][j]);

        double end_time = MPI_Wtime();
        printf("%f\n", end_time - start_time);
    }
    else
    {
        MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        double mat_1_recv[s][n_col];
        double mat_2_recv[s][n_col];
        long double mat_3_recv[s][n_col];
        MPI_Scatter(mat_1, s, MPI_DOUBLE, mat_1_recv, s, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Bcast(&mat_2, n_row * n_col, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        for (int k = 0; k < n_col; k++)
            for (int i = 0; i < s; i++)
            {
                mat_3_recv[i][k] = 0.0;
                for (int j = 0; j < n_col; j++)
                    mat_3_recv[i][k] = mat_3_recv[i][k] + mat_1_recv[i][j] * mat_2[j][k];
            }
        MPI_Gather(mat_3_recv, s, MPI_LONG_DOUBLE, mat_3, s, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}