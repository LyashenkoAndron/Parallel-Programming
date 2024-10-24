#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void time_omp(int * array, int threads, int count) {
    double start = omp_get_wtime();
    int max = -1;

    #pragma omp parallel num_threads(threads) shared(array) reduction(max: max)
    {
        #pragma omp for
        for(int i = 0; i < count; i++)
            if(array[i] > max) { max = array[i]; };
    }

    double end = omp_get_wtime();
    printf("%f    =   %d+++\n", end-start, threads);
    printf("max = %d\n", max);
}


int main(int argc, char** argv)
{
    const int count = 100000000;

    const int threads = 5;
    const int random_seed = 121215;

    int* array = 0;
    int  max   = -1;


    srand(random_seed);

    array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand() % 10000; }

    time_omp(array, threads, count);

    //printf("======\nMax is: %d;\n", max);
    return(0);
}
