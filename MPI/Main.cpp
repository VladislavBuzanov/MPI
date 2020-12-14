#include<mpi.h>
#include<iostream>

int main(int argc, char** argv)
{

    int rank, size, sendBuffer[1], recieveBuffer[1], startNumber = 2;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        sendBuffer[0] = startNumber;
        printf("Круг начинается | Стартовое число = %d | Преобразование: умножение на 2\n", startNumber);
        MPI_Send(sendBuffer, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(recieveBuffer, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, &status);
        sendBuffer[0] = 2 * recieveBuffer[0];

        if (rank != size - 1)
            MPI_Send(sendBuffer, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        else
            MPI_Send(sendBuffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Recv(recieveBuffer, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, &status);
        printf("Круг завершился | Число после преобразований = %d\n", recieveBuffer[0]);
    }

    MPI_Finalize();
}