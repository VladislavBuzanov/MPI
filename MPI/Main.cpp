#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <vector>

int main(int argc, char** argv) {
    int rank, size;
    const int N = 4;
    int matrix[N][N], vector[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int k = N;
    int localArray[N];
    int exitVector[N];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = rand() % 10;
            }
        }
        printf("Matrix :\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d\t", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\nVector :\n");
        for (int i = 0; i < N; i++) {
            vector[i] = rand() % 10;
            printf("%d\t", vector[i]);
        }
        printf("\n\n");
    }

    int linearMatrix[N * N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            linearMatrix[i * N + j] = matrix[j][i];
        }
    }
    
    MPI_Scatter(linearMatrix, k, MPI_INT, localArray, k, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < k; i++)
        localArray[i] *= vector[rank];


    MPI_Reduce(localArray, exitVector, k, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result vector :\n");
        for (int i = 0; i < N; i++) {
            printf(" %d\t", exitVector[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}
