#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {

    const int N = 10;
    int rank, size, localLength;
    double a[N], b[N], localSum = 0, globalSum = 0;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 10;
            b[i] = rand() % 10;
        }

        printf("Вектор А:\n");
        for (int j = 0; j < N; j++) {
            printf("%.0f ", a[j]);
        }
        printf("\n");

        printf("Вектор B:\n");
        for (int j = 0; j < N; j++) {
            printf("%.0f ", b[j]);
        }
        printf("\n");
    }

    int lengths[size];
    int indexes[size];
    int rest = N;
    int k = rest / size;
    lengths[0] = k;
    indexes[0] = 0;
    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        lengths[i] = k;
        indexes[i] = indexes[i - 1] + lengths[i - 1];
    }

    localLength = lengths[rank];
    double localA[localLength];
    double localB[localLength];

    MPI_Scatterv(a, lengths, indexes, MPI_DOUBLE, localA, localLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, lengths, indexes, MPI_DOUBLE, localB, localLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < localLength; i++)
        localSum += localA[i] * localB[i];

    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Результат скалярного произведения = %.0f\n", globalSum);
    }

    MPI_Finalize();
}