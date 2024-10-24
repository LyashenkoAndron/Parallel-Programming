#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int find_max(int * array, int start, int end) {
    int max = -1;
    for (int i = start; i < end; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

int main(const int argc, char** argv) {
    int size = -1;
    int rank = -1;

    const int count = 1e7;
    const int random_seed = 920215;

    double start_time = 0, end_time = 0, global_time = 0;
    int* array = nullptr;
    int lmax = -1;
    int  max = -1;

    int ret = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    array = (int*)malloc(count * sizeof(int));
    if (!rank) {
        srand(random_seed);
        for (int i = 0; i < count; i++) { array[i] = rand(); }
    }

    MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);

    const int wstart = rank * count / size;
    const int wend = (rank + 1) * count / size;

    start_time = MPI_Wtime();

    lmax = find_max(array, wstart, wend);
    MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    MPI_Reduce(&elapsed_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    ret = MPI_Finalize();
    if (!rank) {
        printf("time = %f", global_time);
    }
    return 0;
}
