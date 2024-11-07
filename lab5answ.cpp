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

    int* array = nullptr;
    int lmax = -1;
    int max = -1;

    int ret = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    array = (int*)malloc(count * sizeof(int));
    if (!rank) {
        srand(random_seed);
        for (int i = 0; i < count; i++) { array[i] = rand(); }
    }

    // Устанавливаем количество итераций для усреднения времени
    const int iterations = 10;
    double total_bcast_time = 0.0, total_reduce_time = 0.0;

    for (int i = 0; i < 10; i++) {
        double local_bcast_start = MPI_Wtime();
        MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);
        double local_bcast_end = MPI_Wtime();
        double local_bcast_time = local_bcast_end - local_bcast_start;

        MPI_Reduce(&local_bcast_time, &total_bcast_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        const int wstart = rank * count / size;
        const int wend = (rank + 1) * count / size;
        lmax = find_max(array, wstart, wend);

        double local_reduce_start = MPI_Wtime();
        MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);
        double local_reduce_end = MPI_Wtime();
        double local_reduce_time = local_reduce_end - local_reduce_start;

        MPI_Reduce(&local_reduce_time, &total_reduce_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("Среднее время для Broadcast: %f\n", total_bcast_time / (iterations * size));
        printf("Среднее время для Reduce: %f\n", total_reduce_time / (iterations * size));
    }

    free(array);
    ret = MPI_Finalize();
    return 0;
}
