#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <pthread.h>

using namespace std::chrono;

#define MAX_THREADS 16

void fillRandomMatrix(int matrixSize, int** matrix) {
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }
}

struct ThreadArgs {
    int threadId;
    int partitionSize;
    int matrixSize;
    int** matrix1;
    int** matrix2;
    int** resultMatrix;
};

void *multiplyMatrix(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    int threadId = threadArgs->threadId;
    int partitionSize = threadArgs->partitionSize;
    int matrixSize = threadArgs->matrixSize;
    int** matrix1 = threadArgs->matrix1;
    int** matrix2 = threadArgs->matrix2;
    int** resultMatrix = threadArgs->resultMatrix;

    for (int i = threadId * partitionSize; i < (threadId + 1) * partitionSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            resultMatrix[i][j] = 0;
            for (int k = 0; k < matrixSize; ++k) {
                resultMatrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return NULL;
}

int main() {
    srand(time(0));

    int matrixSize = rand() % 400 + 1; // Generate a random size between 1 and 400

    int** matrix1 = new int*[matrixSize];
    int** matrix2 = new int*[matrixSize];
    int** matrix3 = new int*[matrixSize];

    for (int i = 0; i < matrixSize; ++i) {
        matrix1[i] = new int[matrixSize];
        matrix2[i] = new int[matrixSize];
        matrix3[i] = new int[matrixSize];
    }

    fillRandomMatrix(matrixSize, matrix1);
    fillRandomMatrix(matrixSize, matrix2);

    auto start = high_resolution_clock::now();

    pthread_t matrix_thread[MAX_THREADS];
    ThreadArgs threadArgs[MAX_THREADS];

    int partitionSize = matrixSize / MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; ++i) {
        threadArgs[i] = {i, partitionSize, matrixSize, matrix1, matrix2, matrix3};
        pthread_create(&matrix_thread[i], NULL, &multiplyMatrix, &threadArgs[i]);
    }

    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(matrix_thread[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    printf("Matrix multiplication completed in: %d microseconds\n", duration.count());

    std::ofstream outputFile("p_thread.txt");
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            outputFile << matrix3[i][j] << "\t";
        }
        outputFile << std::endl;
    }
    outputFile << "Execution time: " << duration.count() << " microseconds";
    outputFile.close();

    // Deallocate memory
    for (int i = 0; i < matrixSize; ++i) {
        delete[] matrix1[i];
        delete[] matrix2[i];
        delete[] matrix3[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    delete[] matrix3;

    return 0;
}
