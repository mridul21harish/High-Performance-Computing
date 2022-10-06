#include<omp.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[])
{  
	float start_time; float end_time; float exec_time;
	start_time = omp_get_wtime();
	
	int n = 50000;
	double a[n]; double b[n]; double c = 0;
	
	double random_num(double low, double high)
	{
   		double d = (double)rand() / ((double)RAND_MAX + 1);
    		return (low + d * (high - low));
	}
	
	#pragma omp parallel for reduction(+ : c)
	for(int i = 0; i < n; i = i+1)
	{
		a[i] = random_num(0, 10010000100);
		b[i] = random_num(0, 10010000100);
		
		for(int k = 1; k < n; k = k+1);
		c = c + (a[i]*b[i]);
	}
	
	end_time = omp_get_wtime(); 
	exec_time = end_time - start_time;
	printf("%f\n", exec_time); 
}
