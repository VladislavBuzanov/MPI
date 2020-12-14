#include <mpi.h>
#include <iostream>
#include <cstdlib>

void scatter(int* sendData, int sendDataCount, MPI_Datatype sendDataType, int* recieveData, int recieveDataCount,
    MPI_Datatype recieveDataType, int rootNumber, MPI_Comm communicator) {

    int rank;
    int size;
    MPI_Status status;

    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == rootNumber) {
        for (int i = 0; i < size; i++) {
            if (i != rootNumber)
                MPI_Send(&sendData[i * sendDataCount], sendDataCount, sendDataType, i, 0, communicator);
            else {
                for (int j = 0; j < recieveDataCount; j++) {
                    recieveData[j] = sendData[i * sendDataCount + j];
                }
            }
        }
    }
    else {
        MPI_Recv(&recieveData[0], recieveDataCount, recieveDataType, rootNumber, 0, communicator, &status);
    }
}

void gather(int* sendBuf, int sendDataCount, MPI_Datatype sendDataType, int* recvBuf, int recieveDataCount, MPI_Datatype recieveDataType,
    int rootNumber, MPI_Comm communicator) {

    int rank;
    int size;
    MPI_Status status;

    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == rootNumber) {
        for (int i = 0; i < size; i++) {
            if (i != rootNumber) {
                MPI_Recv(&recvBuf[i * recieveDataCount], recieveDataCount, recieveDataType, i, 0, communicator, &status);
            }
            else {
                for (int j = 0; j < recieveDataCount; j++) {
                    recvBuf[i * recieveDataCount + j] = sendBuf[j];
                }
            }
        }
    }
    else {
        MPI_Send(&sendBuf[0], sendDataCount, sendDataType, rootNumber, 0, communicator);
    }
}

int main(int argc, char* argv[]) {

    int rank, size, N = 8, a[N];

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Массив:\n");
        for (int i = 0; i < N; i++) {
            a[i] = rand() % 10;
            printf("%d\t", a[i]);
        }
        printf("\n");
    }

    int localArray[N / size];
    int exitArray[N];

    scatter(a, N / size, MPI_INT, localArray, N / size, MPI_INT, 0, MPI_COMM_WORLD);
    gather(localArray, N / size, MPI_INT, exitArray, N / size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Номер процесса =  %d | Часть массива: ", rank);
    for (int i = 0; i < N / size; i++) {
        printf("%d ", localArray[i]);
    }
    printf("\n");

    if (rank == 0) {
        printf("Полученный массив:\n");
        for (int j = 0; j < N; j++) {
            printf("%d\t", exitArray[j]);
        }
        printf("\n");
    }

    MPI_Finalize();
}

