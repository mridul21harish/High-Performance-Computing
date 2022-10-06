#include<omp.h>
#include<stdlib.h>
#include<stdio.h>

#define n 10000

int main(int argc, char argv[])
{	
	int omp_rank; 
	int i = 0;
	double  a[n]; double b[n]; double c[n];
	float start_time; float end_time; float exec_time;
	
	start_time = omp_get_wtime();
	#pragma omp parallel private(i) shared (a, b, c)
	{
		#pragma omp for
			for(i = 0; i < n; i = i+1)
			{
				omp_rank = omp_get_thread_num();
				for(int j = 0; j < n; j = j+1);
				a[i] = i * 58.473;
				b[i] = i * 218.893;
				c[i] = a[i] * b[i];
				
				//printf("a[%d] = %lf, b[%d] = %lf, c[%d] = %lf, Thread ID = %d \n", i, a[i], i, b[i], i, c[i], omp_rank);
			}
	}
	end_time = omp_get_wtime(); 
	exec_time = end_time - start_time;
	printf("%f\n", exec_time);
}
