#include <iostream>
#include <stdlib.h>
#include <omp.h>


void shell(int * arr, int count, int n_threads) {
    double start = omp_get_wtime();
    for (int step = count / 2; step > 0; step /= 2) {
        int i, j, tmp;
        #pragma omp parallel for num_threads(n_threads) shared(arr, count) private(i, j, tmp)
        for (i = step; i < count; i++) {
            tmp = arr[i];

            for (j = i; j >= step && arr[j - step] > tmp; j -= step)
                arr[j] = arr[j - step];

            arr[j] = tmp;

        }
    }
    double end = omp_get_wtime();
    printf("%f\n", end - start);
}


int main() {
    const int initial_size = 10000;  ///< Initial number of array elements
    int random_seed = 820205;        ///< RNG seed
    const int n_threads = 8;         ///< Number of threads
    const int iterations = 10;       ///< Number of iterations (with size increase)
    const int multiplier = 4;       ///< Size increase multiplier (can be set to 100)

    int* array = 0;                  ///< Array for sorting

    /* Initialize the RNG */
    srand(random_seed);

    /* Perform iterations with increasing array sizes */
    int size = initial_size;
    for (int iter = 0; iter < iterations; iter++) {
        /* Generate the random array */
        array = (int*)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) { array[i] = rand(); }

        /* Sort the array and measure time */
        shell(array, size, n_threads);

        /* Increase the size of the array for the next iteration */
        size *= multiplier;
        random_seed -= 10;

        /* Free the allocated memory */
        free(array);
    }

    return 0;
}