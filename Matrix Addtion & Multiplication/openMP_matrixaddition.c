#include<omp.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[])
{  
    	float start_time; float end_time; float exec_time;
    	double a[600][1000]; double b[600][1000]; double c[600][1000];     
    	int row = 600, column = 1000; 
    	start_time = omp_get_wtime();
    
    	// Addition
    	#pragma omp parallel for
    	for(int i = 0; i < row; i = i+1)    
    	{    
        	for(int k = 0; k < 100000; k = k+1);
        	#pragma omp parallel for
        	for(int j = 0; j < column; j = j+1) 
        	{    
            		a[i][j] = i+j+12.549;
            		b[i][j] = i+j+97.949;
            		c[i][j] = a[i][j] + b[i][j];    
        	}
    	}  

    	end_time = omp_get_wtime(); 
	exec_time = end_time - start_time;
	printf("%f\n", exec_time);
}

