#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std::chrono;

// Multiply two matrices
void multiplyMatrices(int matrixSize, const int* matrix1, const int* matrix2, int* resultMatrix) {
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            resultMatrix[i * matrixSize + j] = 0;
            for (int k = 0; k < matrixSize; ++k)
                resultMatrix[i * matrixSize + j] += matrix1[i * matrixSize + k] * matrix2[k * matrixSize + j];
        }
    }
}

int main() {
    srand(time(nullptr));

    int matrixSize = rand() % 400 + 1; // Generate a random size between 1 and 400

    int* matrix1 = new int[matrixSize * matrixSize];
    int* matrix2 = new int[matrixSize * matrixSize];
    int* resultMatrix = new int[matrixSize * matrixSize];

    // Fill matrices with random values
    for (int i = 0; i < matrixSize * matrixSize; ++i) {
        matrix1[i] = rand() % 100;
        matrix2[i] = rand() % 100;
    }

    auto startTime = high_resolution_clock::now();
    // Perform matrix multiplication
    multiplyMatrices(matrixSize, matrix1, matrix2, resultMatrix);
    auto stopTime = high_resolution_clock::now();

    // Calculate the duration of the multiplication
    auto duration = duration_cast<microseconds>(stopTime - startTime);

    std::cout << "Matrix multiplication completed in: " << duration.count() << " microseconds" << std::endl;

    // Write the result to a file
    std::ofstream outputFile("sequential.txt");
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j)
            outputFile << resultMatrix[i * matrixSize + j] << "\t";
        outputFile << std::endl;
    }
    outputFile << "Execution time: " << duration.count() << " microseconds";
    outputFile.close();

    delete[] matrix1;
    delete[] matrix2;
    delete[] resultMatrix;

    return 0;
}
