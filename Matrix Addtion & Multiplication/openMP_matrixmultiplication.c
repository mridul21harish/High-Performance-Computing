#include<omp.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[])
{  
	float start_time; float end_time; float exec_time;
	start_time = omp_get_wtime();
	
	double a[500][500]; double b[500][500]; double c[500][500];     
    	int row = 500, column = 500;
	
	//multiplication
	#pragma omp parallel for
	for(int i = 0; i < row; i = i+1)    
    	{   
    		#pragma omp parallel for
    		for(int j = 0; j < column; j = j+1) 
        	{    
        		c[i][j] = 0;
        		a[i][j] = i+j+12.549;
            		b[i][j] = i+j+97.949;
        		for(int k = 0; k < column; k = k+1) 
        		{
        			c[i][j] += a[i][k]*b[k][j];
        		}
        	}	
	 }
	end_time = omp_get_wtime(); 
	exec_time = end_time - start_time;
	printf("%f\n", exec_time);
}


