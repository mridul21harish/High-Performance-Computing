#include<omp.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[])
{  
	float start_time; float end_time; float exec_time;
	start_time = omp_get_wtime();
	
        int sum = 0; 
	int num = 50000;
	 
      	#pragma omp parallel for reduction(+ : sum)
        for (int i = 0; i <= num; i = i+1)  
        {  
        	for(int k = 0; k < num; k = k+1);
            	sum = sum + i; 
        }  
      
        //printf("\n Sum of the first %d number is: %d", num, sum);  
        
        end_time = omp_get_wtime(); 
	exec_time = end_time - start_time;
	printf("%f\n", exec_time); 
}  
