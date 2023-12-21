# Cannons-Matrix-Matrix-Multiplication

## Introduction - What exactly the algo is?

Cannon's algorithm is a parallel algorithm designed for matrix-matrix multiplication on distributed-memory systems. It reduces the communication overhead by organizing the processes in a 2D grid and carefully coordinating the data movement among the processes.

The key idea is to decompose the input matrices into blocks and arrange them in a 2D processor grid. Each processor computes a local block of the result matrix by communicating with its neighboring processors to exchange necessary data. This structured communication pattern reduces the overall communication cost compared to naive matrix multiplication algorithms.

## Video Demonstration

[![Watch the video](https://github.com/SaadARazzaq/Cannons-Matrix-Matrix-Multiplication/blob/main/banner.jpg)](https://github.com/SaadARazzaq/Cannons-Matrix-Matrix-Multiplication/blob/main/Demonstration.mp4)

## Compilation

To compile the program, use the following command:

```bash
mpicc -o matrix_multiply main.c
```

Here,

- mpicc is the MPI compiler wrapper.
- -o matrix_multiply specifies the output executable name.
- main.c is the source code file.

## Execution

To execute the program, use the mpirun command:

```bash
mpiexec -np <number_of_processes> ./matrix_multiply <matrix_dimension>
```

Here,

- <num_processes> is the number of MPI processes you want to run.
- <number_of_elements> is the dimension of the square matrices. Ensure that the matrix dimension is divisible by the square root of the number of processes.

## Example Run

To run the program with 4 MPI processes and 8 dimensions:

```bash
mpicc -o matrix_multiply main.c
mpiexec -np 4 ./matrix_multiply 8
```

## Program Logic

1. Each MPI process initializes local matrices A, B, and C.
2. Matrices A and B are randomly generated on process 0 and broadcasted to all processes.
3. Matrices A and B are decomposed into local blocks and scattered to all processes.
4. Cannon's algorithm is used to perform matrix multiplication in parallel.
5. The result matrix C is gathered from all processes onto process 0.
6. Matrices A, B, and C are printed on process 0 for debugging (optional).

**Note**

**- Ensure that MPI is properly installed on your system.**

**- Adjust the compilation and execution commands based on your MPI setup.**

**- The program expects a command-line argument for the number of elements.**

```bash 
Made with 💖 by Saad and Wasif
```
