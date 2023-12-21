#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

// Function to print a matrix (for debugging)
void printMatrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to perform matrix multiplication using Cannon's algorithm
void cannonMatrixMultiply(int *localA, int *localB, int *localC, int local_n) {
    for (int i = 0; i < local_n; i++) {
        for (int j = 0; j < local_n; j++) {
            for (int k = 0; k < local_n; k++) {
                localC[i * local_n + j] += localA[i * local_n + k] * localB[k * local_n + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n;             // Matrix dimension
    int sqrt_p;        // Square root of the number of processes
    int myrank, p;     // Rank and size of the MPI communicator
    int *A, *B, *C;    // Matrices A, B, and C
    int *localA, *localB, *localC; // Local matrices for each process
    int local_n;       // Dimension of local matrices

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (argc != 2) {
        if (myrank == 0) {
            printf("Usage: %s <matrix_dimension>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    n = atoi(argv[1]);
    sqrt_p = (int)sqrt(p);

    if (n % sqrt_p != 0) {
        if (myrank == 0) {
            printf("Matrix dimension must be divisible by the square root of the number of processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Initialize matrices A, B, and C on all processes
    A = (int *)malloc(n * n * sizeof(int));
    B = (int *)malloc(n * n * sizeof(int));
    C = (int *)calloc(n * n, sizeof(int));

    // Initialize matrices A and B with random values on process 0
    if (myrank == 0) {
        srand(42); // Seed for reproducibility
        for (int i = 0; i < n * n; i++) {
            A[i] = rand() % 10;
            B[i] = rand() % 10;
        }

        // Print the input matrices (for debugging)
        printf("Matrix A:\n");
        printMatrix(A, n, n);
        printf("Matrix B:\n");
        printMatrix(B, n, n);
    }

    // Broadcast the matrix dimension to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the dimension of local matrices
    local_n = n / sqrt_p;

    // Allocate memory for local matrices
    localA = (int *)malloc(local_n * local_n * sizeof(int));
    localB = (int *)malloc(local_n * local_n * sizeof(int));
    localC = (int *)calloc(local_n * local_n, sizeof(int));

    // Scatter blocks of A and B to all processes
    MPI_Scatter(A, local_n * local_n, MPI_INT, localA, local_n * local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, local_n * local_n, MPI_INT, localB, local_n * local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform matrix multiplication using Cannon's algorithm
    cannonMatrixMultiply(localA, localB, localC, local_n);

    // Gather the results from all processes
    MPI_Gather(localC, local_n * local_n, MPI_INT, C, local_n * local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the result matrix (only by process 0 for simplicity)
    if (myrank == 0) {
        printf("Matrix C (Result):\n");
        printMatrix(C, n, n);
    }

    free(A);
    free(B);
    free(C);
    free(localA);
    free(localB);
    free(localC);

    MPI_Finalize();

    return 0;
}
