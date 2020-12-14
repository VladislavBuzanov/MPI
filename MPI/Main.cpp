#include <mpi.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char* argv[]) {

    double localPI, globalPI, pi, x, y, r;
    int rank, size, inCounter = 0, iterations = 10000;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 1; i <= iterations; i++) {
        r = rand() % 1000 / (double)1000;
        x = (2.0 * r) - 1.0;
        r = rand() % 1000 / (double)1000;
        y = (2.0 * r) - 1.0;
        if ((x * x + y * y) <= 1.0)
            inCounter++;
    }

    localPI = 4.0 * (double)inCounter / iterations;

    MPI_Reduce(&localPI, &globalPI, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi = globalPI / size;
        printf("Pi, getted with Monte-Carlo method: %10.8f\n", pi);
        printf("Real Pi: 3.14159265 \n");
    }

    MPI_Finalize();
}