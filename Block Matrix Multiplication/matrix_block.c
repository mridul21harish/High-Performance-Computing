#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define B 10

double random_num(double x)
{
    double num;
    num = (float)rand()/RAND_MAX * x ;
    return num;
}

int main(int argc, char* argv[])
{
    if (argc == 2)
        omp_set_num_threads(atoi(argv[1]));

    float startTime, endTime, execTime;
    int ii, jj, kk, i , j ,k;
    int n = 500; 
    double a[n][n],b[n][n],c[n][n]; 
    startTime = omp_get_wtime();

    #pragma omp parallel for
    for (ii = 0; ii < n; ii+=B)
    {
        for (jj = 0; jj < n; jj+=B)
        {
            for (kk = 0; kk < n; kk+=B)
            {
                for (i = ii; i < ii+B; i++)
                {
                    for (j = jj; j < jj+B; j++)
                    {
                        for (k = kk; k < kk+B; k++)
                        {
                            a[i][j] = random_num(540012301.0);
                            b[i][j] = random_num(205465410.0);
                            c[i][j] = 0;
                            for (int l = 0; l < 100; l++);
                            c[i][j] += a[i][k] * b[k][j];
                        }
                    }
                }
            }
        }
    }
    
    endTime = omp_get_wtime();
    execTime = endTime - startTime;
    printf("%f \n", execTime);
}
