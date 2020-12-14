
#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {

    int rank, size, localLength, numberOfLines = 10, numberOfColumns = 5, localMaxMin, localMinMax, localMin, localMax, maxmin, minmax;

    srand(time(NULL));

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array[numberOfLines * numberOfColumns];

    if (rank == 0) {
        printf("Матрица:\n");
        for (int i = 0; i < numberOfLines; i++) {
            for (int j = 0; j < numberOfColumns; ++j) {
                array[i * numberOfColumns + j] = rand() % 10;
                printf("%d\t", array[i * numberOfColumns + j]);
            }
            printf("\n");
        }
    }

    int lengths[size];
    int indexes[size];
    int rest = numberOfLines;
    int k = rest / size;
    lengths[0] = k * numberOfColumns;
    indexes[0] = 0;
    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        lengths[i] = k * numberOfColumns;
        indexes[i] = indexes[i - 1] + lengths[i - 1];
    }
    localLength = lengths[rank];
    int localArray[localLength];

    MPI_Scatterv(array, lengths, indexes, MPI_INT, localArray, localLength, MPI_INT, 0, MPI_COMM_WORLD);

    localMaxMin = localArray[0];
    localMinMax = localArray[0];
    for (int i = 0; i < localLength / numberOfColumns; i++) {
        localMin = localArray[i * numberOfColumns];
        localMax = localArray[i * numberOfColumns];
        for (int j = 0; j < numberOfColumns; j++) {

            if (localMin > localArray[numberOfColumns * i + j]) {
                localMin = localArray[numberOfColumns * i + j];
            }

            if (localMax < localArray[numberOfColumns * i + j]) {
                localMax = localArray[numberOfColumns * i + j];
            }
        }

        if (localMin > localMaxMin || i == 0) {
            localMaxMin = localMin;
        }

        if (localMax < localMinMax || i == 0) {
            localMinMax = localMax;
        }

    }

    MPI_Reduce(&localMaxMin, &maxmin, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localMinMax, &minmax, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Максимальное значение из минимальных = %d\n", maxmin);
        printf("Минимальное значение из максимальных = %d\n", minmax);
        if (maxmin == minmax) {
            printf("MINMAX = MAXMIN");
        }
        else {
            printf("MINMAX != MAXMIN\n");
        }
    }

    MPI_Finalize();
}