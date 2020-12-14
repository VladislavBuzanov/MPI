#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {

    const int N = 10;
    int rank, size, processLength, localCounter = 0, randomNumber;
    double array[N], localSum = 0;
    double localArray[2], globalArray[2];

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Array:");
        for (int i = 0; i < N; i++) {
            randomNumber = rand() % 99 - 50;
            array[i] = randomNumber;
            printf(" %d ", randomNumber);
        }
        printf("\n");
    }
    
    int *lengths = new int[size];
    int *indexes = new int[size];
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

    processLength = lengths[rank];
    
    double *processArray = new double[processLength];

    MPI_Scatterv(array, lengths, indexes, MPI_DOUBLE, processArray, processLength, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < processLength; i++)
        if (processArray[i] > 0) {
            localSum += processArray[i];
            localCounter++;
        }

    localArray[0] = localSum;
    localArray[1] = localCounter;

    MPI_Reduce(localArray, globalArray, 2, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Avg = %.2f\n", globalArray[0] / globalArray[1]);
    }

    MPI_Finalize();
}