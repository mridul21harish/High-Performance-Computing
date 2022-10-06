#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define n_row 200
#define n_col 200
double mat_1[n_row][n_col], mat_2[n_row][n_col], mat_3[n_row][n_col];
double mat_1_recv[n_row][n_col], mat_2_recv[n_row][n_col], mat_3_recv[n_row][n_col];

int main(int argc, char *argv[])
{
    int myid, numprocs, s, rows, index = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    //fprintf(stderr, "Process %d on %s\n", myid, processor_name);
    if (myid == 0)
    {
        for (int i = 0; i < n_row; i++)
            for (int j = 0; j < n_col; j++)
            {
                mat_1[i][j] = i + j;
                mat_2[i][j] = i + j;
            }
        int i;
        if (numprocs == 1)
            s = n_row;
        else
            s = n_row / (numprocs - 1);
        if (numprocs > 1)
        {
            for (i = 1; i < numprocs - 1; i++)
            {
                MPI_Send(&s, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&index, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&mat_1[index][0], s * n_col, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                MPI_Send(&mat_2, n_row * n_col, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                index += s;
            }
            int elements_left = n_row - index;
            MPI_Send(&elements_left, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&index, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&mat_1[index][0], elements_left * n_col, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&mat_2, n_row * n_col, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        for (i = 1; i < numprocs; i++)
        {
            int rows;
            MPI_Recv(&index, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&mat_3[index][0], rows * n_col, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            int sender = status.MPI_SOURCE;
        }

        // for (int i = 0; i < n_row; i++)
        //     for (int j = 0; j < n_col; j++)
        //         printf("%d %d %f %f %Lf\n", i, j, mat_1[i][j], mat_2[i][j], mat_3[i][j]);

        double end_time = MPI_Wtime();
        printf("%lf\n", end_time - start_time);
    }

    else
    {
        MPI_Recv(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&index, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);
        MPI_Recv(&mat_1_recv, rows * n_col, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&mat_2_recv, n_row * n_col, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        for (int k = 0; k < n_col; k++)
            for (int i = 0; i < rows; i++)
            {
                mat_3_recv[i][k] = 0.0;
                for (int j = 0; j < n_col; j++)
                    mat_3_recv[i][k] = mat_3_recv[i][k] + mat_1_recv[i][j] * mat_2_recv[j][k];
            }
        MPI_Send(&index, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&mat_3_recv, rows * n_col, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}