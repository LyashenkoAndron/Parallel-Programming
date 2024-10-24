#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void time_omp(int * array, int threads, int count) {
    double start = omp_get_wtime();
    int max = -1;

    #pragma omp parallel num_threads(threads) shared(array) reduction(max: max)
    {
        #pragma omp for schedule(guided, /*param*/)
        for(int i = 0; i < count; i++)
            if(array[i] > max) { max = array[i]; };
    }

    double end = omp_get_wtime();
    printf("%f\n", end-start);
    //printf("max = %d\n", max);
}


int main(int argc, char** argv)
{
    const int count = 100000000;
    const int random_seed = 321215;

    int* array = 0;
    int  max   = -1;


    srand(random_seed);

    array = (int*)malloc(count*sizeof(int));

    for(int i=0; i<count; i++) { array[i] = rand() % 10000; }

    for (int i = 1; i < 21; i++) {
        time_omp(array, i, count);
    }

    return(0);
}
