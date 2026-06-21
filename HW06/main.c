
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    long N = 1000;
    long chunk_size = 128;
    double a = 3.0, xval = 0.1, yval = 7.1;

    double *x = malloc(N*sizeof(double));
    double *y = malloc(N*sizeof(double));
    double *d = malloc(N*sizeof(double));

    for(long i=0;i<N;i++){
        x[i]=xval;
        y[i]=yval;
    }

    long num_chunks = (N + chunk_size - 1)/chunk_size;
    double *partial = calloc(num_chunks,sizeof(double));

    for(long c=0;c<num_chunks;c++){
        long start = c*chunk_size;
        long end = start + chunk_size;
        if(end>N) end=N;

        for(long i=start;i<end;i++){
            d[i] = a*x[i] + y[i];
            partial[c] += d[i];
        }
    }

    double total=0.0, ref=0.0;
    for(long c=0;c<num_chunks;c++) total += partial[c];
    for(long i=0;i<N;i++) ref += d[i];

    printf("Chunk sum = %.15f\n", total);
    printf("Reference = %.15f\n", ref);

    free(x); free(y); free(d); free(partial);
    return 0;
}
