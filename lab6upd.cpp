#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(const int argc, char** argv) {
    int size = -1;
    int rank = -1;

    const int random_seed = 920214;
    double start_time = 0, end_time = 0, global_time = 0;

    const int count = 100000;
    int* array = nullptr;

    int ret = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    array = (int*)malloc(count * sizeof(int));
    if (!rank) {
        srand(random_seed);
        for (int i = 0; i < count; i++) { array[i] = rand(); }
    }

    int local_count = count / size;
    int *local_array = (int*)malloc(local_count * sizeof(int));

    start_time = MPI_Wtime();

    for (int step = count / 2; step > 0; step /= 2) {
        int i, j, tmp;

        int chunk = (count - step) / size;
        int start = step + rank * chunk;
        int end = step + (rank + 1) * chunk;
        if (rank == size - 1) { end = count; }

        for (i = start; i < end; i++) {
            tmp = array[i];

            for (j = i; j >= step && array[j - step] > tmp; j -= step)
                array[j] = array[j - step];

            array[j] = tmp;

        }
    }

    end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    MPI_Reduce(&elapsed_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    ret = MPI_Finalize();

    if (!rank) {
        printf("time %f", global_time);
        /*for (int i = 0; i < count; i++) {
            printf("%d ", array[i]);
            if (i != 0 and i % 10 == 0) {printf("\n");}
        }*/
    }

    free(array);
    free(local_array);
    return 0;
}
