#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {

    int rank, size;
    int const N = 10;
    int array[N];
    int exitArray[N];

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("Array:\n");
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 100;
            printf(" %d ", array[i]);
        }
        printf("\n");
    }

    int *lengths = new int[size];
    int *indexes = new int[size];
    int *reversedIndexes = new int[size];

    int rest = N;
    int k = rest / size;
    lengths[0] = k;
    indexes[0] = 0;
    reversedIndexes[0] = N - k;

    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        lengths[i] = k;
        indexes[i] = indexes[i - 1] + lengths[i - 1];
        reversedIndexes[i] = reversedIndexes[i - 1] - lengths[i];
    }
    int localLength = lengths[rank];
    int *localArray = new int[localLength];

    MPI_Scatterv(array, lengths, indexes, MPI_INT, localArray, localLength, MPI_INT, 0, MPI_COMM_WORLD);

    int *reversedLocalArray = new int[localLength];
    for (int i = 0; i < localLength; i++) {
        reversedLocalArray[i] = localArray[localLength - i - 1];
    }

    MPI_Gatherv(reversedLocalArray, localLength, MPI_INT, exitArray, lengths, reversedIndexes, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Reversed array :\n");
        for (int i = 0; i < N; i++) {
            printf(" %d ", exitArray[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}
