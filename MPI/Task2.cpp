#include <cstdio>
#include <mpi.h>
#include <iostream>

void task2(int argc, char* argv[])
{
    const int N = 100;
    int rank, num;
    double array[N], globalMax = 0.0, currentMax = 0.0;
    MPI_Status mpi_status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 99;
        }
    }

    MPI_Bcast(array, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int k = N / num;
    int i1 = k * rank;
    int i2 = k * (rank + 1);

    if (rank == num - 1) {
        i2 = N;
    }

    for (int i = i1; i < i2; i++) {
        currentMax = std::max(currentMax, array[i]);
    }

    if (rank == 0) {
        globalMax = currentMax;
        for (int i = 1; i < num; i++) {
            MPI_Recv(&currentMax, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &mpi_status);
            globalMax = std::max(globalMax, currentMax);
        }
    }
    else {
        MPI_Send(&currentMax, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("Max = %.1f\n", globalMax);
    }

    MPI_Finalize();
}