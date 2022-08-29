// GEMM_pthread.c

// Copyright (c) 2021 CHEN Yuhan
// Date: 2021-10-28
//
// pthread ver of GEMM
// sorry but float type matrix only
// COPE WITH HOMEWORK ONLY

// Compile: gcc -pthread -o GEMM_pthread GEMM_pthread.c
// Run:     ./GEMM_pthread

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define THREAD_NUM 8 
#define SIZE 2048
#define AVG_ROWS (SIZE / THREAD_NUM)
#define SHOW_MATRIX 0
#define CHECK 0
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

float *A, *B, *C;
int    M = SIZE, N = SIZE, K = SIZE, m, n, k;

void  matrix_multiply(int M, int N, int K, float *A, float *B, float *C);
void  PrintMatrix(float *Matrix, const int R, const int C);
void *thread_GEMM(void *arg) {
    int num_of_thread = (int)arg;
    int start, end;
    start = AVG_ROWS * num_of_thread;
    end   = AVG_ROWS * (num_of_thread + 1) - 1;

    // printf("Thread #%d: start:%d end:%d\n", num_of_thread, start, end);
    matrix_multiply(AVG_ROWS, SIZE, SIZE, A + (start * N), B, C + (start * K));

    pthread_exit(0);
}

int main() {

    clock_t    startTime, endTime;
    pthread_t *pthread_id = NULL;
    long long  i;

    int begin_Arow, end_Arow, avg_rows;

    A = (float *)malloc(sizeof(float) * M * N);
    B = (float *)malloc(sizeof(float) * N * K);
    C = (float *)malloc(sizeof(float) * M * K);

    pthread_id = (pthread_t *)malloc(THREAD_NUM * sizeof(pthread_t));

    // printf("avgr:%d\n", avg_rows);
    printf("\nAllocating memory for matrices\n\n");
    for (m = 0; m < M; m++) {
        for (n = 0; n < N; n++) {
            A[m * N + n] = (float)(rand() / (rand() / 100));
        }
    }
    for (n = 0; n < N; n++) {
        for (k = 0; k < K; k++) {
            B[n * K + k] = (float)(rand() / (rand() / 100));
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        pthread_create(pthread_id + i, NULL, thread_GEMM, i);
    }
    startTime = clock();
    for (i = 0; i < THREAD_NUM; i++) {
        pthread_join(pthread_id[i], NULL);
    }
    endTime = clock();
    printf("Total time: %lfs\n", (double)(endTime - startTime) / CLOCKS_PER_SEC);

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

    free(A);
    free(B);
    free(C);

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

// GEMM
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
