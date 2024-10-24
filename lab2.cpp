#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


void find_elem(int * array, int count, int target, int n_threads) {
    int index = -1;
    bool find_flag = false;
    double start = omp_get_wtime();

    #pragma omp parallel num_threads(n_threads)
    {
        #pragma omp for
        for (int i = 0; i < count; i++) {
            if (array[i] == target && !find_flag) {
                #pragma omp critical
                {
                    find_flag = true;
                    index = i;
                }
            }
        }
    }
    double end = omp_get_wtime();
    //printf("Found occurence of %d at index %d;\t", target, index);
    //printf("time: %f, threads: %d\n", end - start, n_threads);
    printf("%f\n", end - start);
}

int main(int argc, char** argv)
{
    const int count = 100000000;     ///< Number of array elements
    int random_seed = 820205; ///< RNG seed
    const int target = 16;          ///< Number to look for

    int* array = 0;                 ///< The array we need to find the max in


    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    for (int i = 1; i <= 20; i++) {
        random_seed -= 10;
        for(int i=0; i<count; i++) { array[i] = rand(); }
        find_elem(array, count, target, i);
    }
    return 0;
}