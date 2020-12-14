#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv) {

    int rank, size;
    int const N = 100000;
    int array[N];
    double startSend, endSend;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = rand() % 100;
        }

        startSend = MPI_Wtime();
        MPI_Send(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(array, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        endSend = MPI_Wtime();
        printf("Время выполнения | Send | %f\n", endSend - startSend);

        startSend = MPI_Wtime();
        MPI_Ssend(array, N, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(array, N, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        endSend = MPI_Wtime();
        printf("Время выполнения | SSend | %f\n", endSend - startSend);

        startSend = MPI_Wtime();
        MPI_Rsend(array, N, MPI_INT, 1, 3, MPI_COMM_WORLD);
        MPI_Recv(array, N, MPI_INT, 1, 3, MPI_COMM_WORLD, &status);
        endSend = MPI_Wtime();
        printf("Время выполнения | RSend | %f\n", endSend - startSend);

    }
    else {
        MPI_Recv(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(array, N, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(array, N, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Ssend(array, N, MPI_INT, 0, 1, MPI_COMM_WORLD);

        MPI_Recv(array, N, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
        MPI_Rsend(array, N, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}