#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <omp.h>

using namespace std::chrono;

void fillRandomMatrix(int** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = rand() % 100;
        }
    }
}

int main() {
    srand(time(nullptr));

    int MATRIX_SIZE = rand() % 400 + 1; // Random size between 1 and 400

    int** matrix1 = new int*[MATRIX_SIZE];
    int** matrix2 = new int*[MATRIX_SIZE];
    int** matrix3 = new int*[MATRIX_SIZE];

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        matrix1[i] = new int[MATRIX_SIZE];
        matrix2[i] = new int[MATRIX_SIZE];
        matrix3[i] = new int[MATRIX_SIZE];
    }

    fillRandomMatrix(matrix1, MATRIX_SIZE);
    fillRandomMatrix(matrix2, MATRIX_SIZE);

    auto start = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix3[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    printf("Matrix multiplication completed in: %d microseconds\n", duration.count());

    std::ofstream outputFile("openMp.txt");
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            outputFile << matrix3[i][j] << "\t";
        }
        outputFile << std::endl;
    }
    outputFile << "Execution time: " << duration.count() << " microseconds";
    outputFile.close();

    // Free dynamically allocated memory
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        delete[] matrix1[i];
        delete[] matrix2[i];
        delete[] matrix3[i];
    }
    delete[] matrix1;
    delete[] matrix2;
    delete[] matrix3;

    return 0;
}
