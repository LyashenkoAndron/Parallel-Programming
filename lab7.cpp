#include "stdio.h"
#include "mpi.h"
#include "omp.h"
#include "math.h"


int get_count_primes_OMP(int tmp, int num_threads) {
    bool flag = true;
    #pragma omp parallel for num_threads(num_threads) shared(tmp, flag) default(none)
    for (int i = 2; i <= sqrt(tmp) && flag; i++) {
        if (tmp % i == 0) {
            flag = false;
        }
    }
    return flag;
}

double get_primes_MPI(int start, int end, int num_threads, int *global_cnt) {
    double start_time = MPI_Wtime();
    int local_cnt = 0;

    for (int i = start; i < end; i++)
        local_cnt += get_count_primes_OMP(i, num_threads);

    double end_time = MPI_Wtime();
    MPI_Reduce(&local_cnt, global_cnt, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return end_time - start_time;
}


int main(const int argc, char** argv) {
    int size = -1;
    int rank = -1;

    int N1 = 1;
    int N2 = 10e6 * 2;
    int step = (N2 - N1) / 8;

    int ret = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int start = N1 + step * rank;
    int end = start + step;
    int global_cnt = 0;
    if (rank == size - 1) { end = N2; }
    double elapsed_time = get_primes_MPI(start, end, 2, &global_cnt);

    if (!rank) printf("cnt = %d\n", global_cnt);
    printf("time: %f - rank: %d", elapsed_time, rank);
    MPI_Finalize();
    return 0;
}