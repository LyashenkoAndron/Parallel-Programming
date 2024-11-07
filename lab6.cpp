#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
// нельзя использовать vector


void shellSort(int * arr, int count) {
    for (int step = count / 2; step > 0; step /= 2) {
        int i, j, tmp;
        for (i = step; i < count; i++) {
            tmp = arr[i];

            for (j = i; j >= step && arr[j - step] > tmp; j -= step)
                arr[j] = arr[j - step];

            arr[j] = tmp;

        }
    }

}

void InsertionSort(int* array, int count) {
    int i, key, j;
    for (i = 1; i < count; i++) {
        key = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

int binarySearch(int a[], int item, int low, int high) {
    if (high <= low)
        return (item > a[low]) ? (low + 1) : low;

    int mid = (low + high) / 2;
    if (item == a[mid])
        return mid + 1;

    if (item > a[mid])
        return binarySearch(a, item,mid + 1, high);
    return binarySearch(a, item, low, mid - 1);
}

int main(const int argc, char** argv) {
    int size = -1;
    int rank = -1;

    const int random_seed = 920214;
    double start_time = 0, end_time = 0, global_time = 0;

    const int count = 10000;
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

    MPI_Scatter(array, local_count, MPI_INT, local_array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    shellSort(local_array, local_count);

    MPI_Gather(local_array, local_count, MPI_INT, array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank) { InsertionSort(array, count); }

    end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    MPI_Reduce(&elapsed_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    ret = MPI_Finalize();

    if (!rank) {
        printf("time %f", global_time);
    }

    free(array);
    free(local_array);
    return 0;
}
