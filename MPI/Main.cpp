
#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]) { 
    int size, rank, recv;
    MPI_Status mpiStatus;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("Hello World! | from process %d\n", rank);
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                MPI_COMM_WORLD, &mpiStatus);
            printf("Hello World! | from process %d\n", recv);
        }
    }
    else
    {
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
};