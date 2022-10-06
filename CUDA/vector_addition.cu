/*
nvcc -o va vector_addition.cu
./va <N> <NUM_BLOCKS>
*/

#include <stdio.h>
#include <stdlib.h>

__global__ void add(double *a, double *b, double *c)
{
    	int index = blockIdx.x * blockDim.x + threadIdx.x;
    	c[index] = a[index] + b[index];
}

int main(int argc, char *argv[])
{
    	int N = 1048576;
    	int NUM_BLOCKS = atoi(argv[1]);

    	FILE *fp;
    	fp = fopen("vectorAddition.txt", "w");
    	double *a, *b, *c;
    	double *d_a, *d_b, *d_c;
    	double size = N * sizeof(double);

    	cudaMalloc((void **)&d_a, size);
    	cudaMalloc((void **)&d_b, size);
    	cudaMalloc((void **)&d_c, size);

    	a = (double *)malloc(size);
    	b = (double *)malloc(size);
    	c = (double *)malloc(size);

    	for (int i = 0; i < N; i++)
    	{
        	a[i] = i;
        	b[i] = i;
    	}

    	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    	add<<<NUM_BLOCKS, N / NUM_BLOCKS>>>(d_a, d_b, d_c);

    	cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    	for (int i = 0; i < N; i++)
        	fprintf(fp, "%f %f %f\n", a[i], b[i], c[i]);

    	free(a);
    	free(b);
    	free(c);
    	cudaFree(d_a);
    	cudaFree(d_b);
    	cudaFree(d_c);
    	fclose(fp);
    	return 0;
}
