// GEMM_MPIv2.cpp

// Copyright (c) 2021 CHEN Yuhan
// Date: 2021-10-08
//
// MPI ver of GEMM, improve from GEMM_MPIv1.cpp, using Scatter, Gather and Bcast
// sorry but float type matrix only

// Compile: mpicxx -g -Wall -o GEMM_MPIv2 GEMM_MPIv2.cpp
// Run:     mpiexec -n <comm_sz> ./GEMM_MPIv2

#include <mpi.h>
#include <stdio.h>
#include <time.h>

#define MAT_SIZE 2048
#define SHOW_MATRIX 1
#define CHECK 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void matrix_multiply(int M, int N, int K, float *A, float *B, float *C);
void PrintMatrix(float *Matrix, const int R, const int C);

int main() {
    int M = MAT_SIZE, N = MAT_SIZE, K = MAT_SIZE, m, n, k, i;
    // struct size{
    //     int _M,_N,_K;
    // };
    float * A, *B, *C, *localA, *localB, *localC;
    clock_t startTime, endTime;
    double  MPI_Start, MPI_End;

    int comm_sz, my_rank;

    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);
    MPI_Status status;
    int        begin_Arow, end_Arow, avg_rows;
    if (comm_sz > 1) avg_rows = M / comm_sz;

    // use vecs to reperesent matrices
    // float **A = (float **)malloc(sizeof(float *) * M);
    // float **B = (float **)malloc(sizeof(float *) * N);
    // float **C = (float **)malloc(sizeof(float *) * M);
    A = (float *)malloc(sizeof(float) * M * N);
    B = (float *)malloc(sizeof(float) * N * K);
    C = (float *)malloc(sizeof(float) * M * K);

    // cout << "Intializing matrix data" << endl
    // << endl;
    if (my_rank == 0) {
        printf("avgr:%d\n", avg_rows);
        printf("\nAllocating memory for matrices\n\n");
        for (m = 0; m < M; m++) {
            // A[m] = (float *)malloc(sizeof(float) * N);
            for (n = 0; n < N; n++) {
                A[m * N + n] = static_cast<float>(rand()) / (static_cast<float>(rand() / 100));
            }
        }
        for (n = 0; n < N; n++) {
            // B[n] = (float *)malloc(sizeof(float) * K);
            for (k = 0; k < K; k++) {
                B[n * K + k] = static_cast<float>(rand()) / (static_cast<float>(rand() / 100));
            }
        }
        // for (m = 0; m < M; m++) {
        //     // C[m] = (float *)malloc(sizeof(float) * K);
        // }

        // cout << "Computing matrix product using GEMM" << endl
        // << endl;
        MPI_Start = MPI_Wtime();
    }

    if (comm_sz == 1) { // serial
        // GEMM
        matrix_multiply(m, n, k, A, B, C);

    } else { // parallel

        // for (i = 0; i < comm_sz - 1; i++) {
        //     begin_Arow = avg_rows * i, end_Arow = i + 1 == comm_sz - 1 ? MAX(M, avg_rows * (i + 1)) : avg_rows * (i + 1);
        //     // printf("0bg%d endr%d\n", begin_Arow, end_Arow);
        //     MPI_Send(&end_Arow, 1, MPI_INT, i + 1, 10, MPI_COMM_WORLD);
        //     MPI_Send(&A[begin_Arow * N], (end_Arow - begin_Arow) * N, MPI_FLOAT, i + 1, 1, MPI_COMM_WORLD); // some rows of matrix A
        //     MPI_Send(B, N * K, MPI_FLOAT, i + 1, 2, MPI_COMM_WORLD);                                        // whole matrix B
        // }
        localA = (float *)malloc(sizeof(float) * avg_rows * N);
        // localB = (float *)malloc(sizeof(float) * N * K);
        localC = (float *)malloc(sizeof(float) * avg_rows * K);

        // printf("#%d scattering.\n", my_rank);
        MPI_Scatter(A, avg_rows * N, MPI_FLOAT, localA, avg_rows * N, MPI_FLOAT, 0, MPI_COMM_WORLD);
        // printf("#%d Bcasting.\n", my_rank);
        MPI_Bcast(B, N * K, MPI_FLOAT, 0, MPI_COMM_WORLD);

        matrix_multiply(avg_rows, N, K, localA, B, localC);

        // printf("#%d Gathering.\n", my_rank);
        MPI_Gather(localC, avg_rows * K, MPI_FLOAT, C, avg_rows * K, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }
    if (my_rank == 0) {
        MPI_End = MPI_Wtime();
#if CHECK
        startTime = clock();
        matrix_multiply(m, n, k, A, B, C);
        endTime = clock();
        printf("\nUsing GEMM\nTop left corner of matrix C:\n");
        PrintMatrix(C, M, K);
        // cout << "Total time: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl
        //      << endl
        //      << "Deallocating memory " << endl
        //      << endl;
        printf("Total time: %lfs\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
#endif
        // for (i = 0; i < comm_sz - 1; i++) {
        //     begin_Arow = avg_rows * i, end_Arow = i + 1 == comm_sz - 1 ? MAX(M, avg_rows * (i + 1)) : avg_rows * (i + 1);
        //     MPI_Recv(&C[begin_Arow * N], (end_Arow - begin_Arow) * K, MPI_FLOAT, i + 1, 3, MPI_COMM_WORLD, &status);
        // }

        // cout << "Computations completed." << endl
        // << endl;
#if SHOW_MATRIX
        // cout << "Top left corner of matrix A:" << endl;
        printf("Top left corner of matrix A:\n");
        PrintMatrix(A, M, N);
        // cout << "Top left corner of matrix B:" << endl;
        printf("Top left corner of matrix B:\n");
        PrintMatrix(B, N, K);
        // cout << "Top left corner of matrix C:" << endl;
        printf("Top left corner of matrix C:\n");
        PrintMatrix(C, M, K);
#endif
        // cout << "Total time: " << (double)(MPI_End - MPI_Start) << "s" << endl
        //      //  << endl
        //      //  << "Deallocating memory " << endl
        //      << endl;
        printf("Total time: %lfs\n", (double)(MPI_End - MPI_Start));

        // for (m = 0; m < M; m++) {
        //     free(A[m]);
        //     free(C[m]);
        // }
        // for (n = 0; n < N; n++) {
        //     free(B[n]);
        // }
        free(A);
        free(B);
        free(C);
    }
    free(localA);
    free(localC);

    // cout << "Completed." << endl
    //      << endl;

    // if (my_rank != 0) {
    //     MPI_Recv(&end_Arow, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
    //     begin_Arow = avg_rows * (my_rank - 1); // end_Arow = MIN(K, avg_rows * (my_rank + 1));

    //     printf("rank%d:\nfrom %d to %d\n", my_rank, begin_Arow, end_Arow);

    //     localA = (float *)malloc(sizeof(float) * (end_Arow - begin_Arow) * N);
    //     localB = (float *)malloc(sizeof(float) * N * K);
    //     localC = (float *)malloc(sizeof(float) * (end_Arow - begin_Arow) * K);

    //     MPI_Recv(localA, (end_Arow - begin_Arow) * N, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
    //     MPI_Recv(localB, N * K, MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);

    //     matrix_multiply(end_Arow - begin_Arow, N, K, localA, localB, localC);
    //     // matrix_multiply(end_Arow - begin_Arow, N, K, localA, B, localC);

    //     MPI_Send(localC, (end_Arow - begin_Arow) * K, MPI_FLOAT, 0, 3, MPI_COMM_WORLD);

    //     free(localA);
    //     // free(localB);
    //     free(localC);
    // }

    MPI_Finalize();

    return 0;
}

// Print the matrix
void PrintMatrix(float *Matrix, const int R, const int C) {
    for (int r = 0; r < MIN(5, R); r++) {
        for (int c = 0; c < MIN(5, C); c++) {
            // cout << Matrix[r * C + c] << " ";
            printf("%f ", Matrix[r * C + c]);
        }
        // cout << endl;
        printf("\n");
    }
    // cout << endl;
    printf("\n");
}

void matrix_multiply(int M, int N, int K, float *A, float *B, float *C) {
    for (int m = 0; m < M; m++) {
        for (int k = 0; k < K; k++) {
            C[m * K + k] = 0;
            for (int n = 0; n < N; n++) {
                C[m * K + k] += A[m * N + n] * B[n * K + k];
            }
        }
    }
}