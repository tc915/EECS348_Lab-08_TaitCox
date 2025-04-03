#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>   // for std::setw
#include <stdexcept> // for exception handling
#include <limits>    // for numeric_limits

// define a type alias
using Matrix = std::vector<std::vector<int>>;

// function declarations
bool loadMatrices(const std::string &filename, Matrix &matrixA, Matrix &matrixB, int &n);
void printMatrix(const Matrix &matrix, const std::string &label);
Matrix addMatrices(const Matrix &matrixA, const Matrix &matrixB);
Matrix multiplyMatrices(const Matrix &matrixA, const Matrix &matrixB);
void sumDiagonals(const Matrix &matrix);
void swapRows(Matrix &matrix, int row1, int row2);
void swapCols(Matrix &matrix, int col1, int col2);
void updateElement(Matrix &matrix, int row, int col, int newValue);

// main function
int main()
{
    std::string filename;
    Matrix matrixA, matrixB;
    int n = 0;

    std::cout << "Enter the input filename: ";
    std::cin >> filename;

    if (!loadMatrices(filename, matrixA, matrixB, n))
    {
        std::cerr << "Error loading matrices from file: " << filename << std::endl;
        return 1;
    }

    std::cout << "\nMatrices loaded" << std::endl;
    printMatrix(matrixA, "Matrix A:");
    printMatrix(matrixB, "Matrix B:");

    std::cout << "\nMatrix Addition" << std::endl;
    Matrix sumMatrix = addMatrices(matrixA, matrixB);
    printMatrix(sumMatrix, "Result (A + B):");

    std::cout << "\nMatrix Multiplication" << std::endl;
    Matrix productMatrix = multiplyMatrices(matrixA, matrixB);
    printMatrix(productMatrix, "Result (A * B):");

    std::cout << "\nDiagonal Sums (Matrix A)" << std::endl;
    sumDiagonals(matrixA);

    // interactive or fixed swaps/updates
    std::cout << "\nSwapping Rows 0 and 1 of Matrix A" << std::endl;
    Matrix matrixA_copy_rows = matrixA;
    swapRows(matrixA_copy_rows, 0, 1);
    printMatrix(matrixA_copy_rows, "Matrix A after row swap:");

    std::cout << "\nSwapping Columns 1 and 2 of Matrix B" << std::endl;
    Matrix matrixB_copy_cols = matrixB; // Work on a copy
    swapCols(matrixB_copy_cols, 1, 2);
    printMatrix(matrixB_copy_cols, "Matrix B after column swap:");

    std::cout << "\nUpdating Element (2, 2) in Matrix A to 99" << std::endl;
    Matrix matrixA_copy_update = matrixA; // Work on a copy
    updateElement(matrixA_copy_update, 2, 2, 99);
    printMatrix(matrixA_copy_update, "Matrix A after update:");

    return 0; // Indicate success
}

// function implementations

/**
 * @brief loads two NxN matrices from a file
 * @param filename the name of the file to read from
 * @param matrixA reference to the first matrix to load into
 * @param matrixB reference to the second matrix to load into
 * @param n reference to store the size N
 * @return true if successful, false otherwise
 */
bool loadMatrices(const std::string &filename, Matrix &matrixA, Matrix &matrixB, int &n)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    // read the size N
    if (!(inFile >> n) || n <= 0)
    {
        std::cerr << "Error: Invalid or missing matrix size N in file" << std::endl;
        inFile.close();
        return false;
    }

    // resize matrices
    matrixA.resize(n, std::vector<int>(n));
    matrixB.resize(n, std::vector<int>(n));

    // read matrix A
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (!(inFile >> matrixA[i][j]))
            {
                std::cerr << "Error: Failed to read element for Matrix A at [" << i << "][" << j << "]" << std::endl;
                inFile.close();
                return false;
            }
        }
    }

    // read matrix B
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (!(inFile >> matrixB[i][j]))
            {
                std::cerr << "Error: Failed to read element for Matrix B at [" << i << "][" << j << "]" << std::endl;
                inFile.close();
                return false;
            }
        }
    }

    inFile.close();
    return true;
}

/**
 * @brief prints a matrix with aligned columns
 * @param matrix the matrix to print
 * @param label a label to print before the matrix
 */
void printMatrix(const Matrix &matrix, const std::string &label)
{
    std::cout << label << std::endl;
    if (matrix.empty() || matrix[0].empty())
    {
        std::cout << "[Empty Matrix]" << std::endl;
        return;
    }

    int n = matrix.size();
    int fieldWidth = 6;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            std::cout << std::setw(fieldWidth) << matrix[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief adds two matrices
 * @param matrixA the first matrix
 * @param matrixB the second matrix
 * @return the resulting sum matrix. throws runtime_error if dimensions don't match
 */
Matrix addMatrices(const Matrix &matrixA, const Matrix &matrixB)
{
    if (matrixA.empty() || matrixA.size() != matrixB.size() || matrixA[0].size() != matrixB[0].size())
    {
        throw std::runtime_error("Matrix dimensions must match for addition");
    }

    int n = matrixA.size();
    Matrix result(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            result[i][j] = matrixA[i][j] + matrixB[i][j];
        }
    }
    return result;
}

/**
 * @brief multiplies two matrices
 * @param matrixA the first matrix
 * @param matrixB the second matrix
 * @return the resulting product matrix. throws runtime_error if dimensions are incompatible
 */
Matrix multiplyMatrices(const Matrix &matrixA, const Matrix &matrixB)
{
    if (matrixA.empty() || matrixA[0].size() != matrixB.size())
    {
        throw std::runtime_error("Matrix dimensions incompatible for multiplication (A's cols must equal B's rows)");
    }

    int n = matrixA.size();    // rows in A and Result
    int m = matrixB[0].size(); // columns in B and Result
    int p = matrixB.size();    // columns in A and Rows in B

    Matrix result(n, std::vector<int>(m, 0)); // initialize result with zeros

    for (int i = 0; i < n; ++i)
    { // iterate rows of Result (and A)
        for (int j = 0; j < m; ++j)
        { // Iterate columns of Result (and B)
            for (int k = 0; k < p; ++k)
            { // iterate common dimension
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    return result;
}

/**
 * @brief calculates and prints the sum of main and secondary diagonals
 * @param matrix the input matrix
 */
void sumDiagonals(const Matrix &matrix)
{
    if (matrix.empty() || matrix.size() != matrix[0].size())
    {
        std::cerr << "Error: Matrix must be square to calculate diagonals" << std::endl;
        return;
    }

    int n = matrix.size();
    long long mainDiagonalSum = 0;
    long long secondaryDiagonalSum = 0;

    for (int i = 0; i < n; ++i)
    {
        mainDiagonalSum += matrix[i][i];
        secondaryDiagonalSum += matrix[i][n - 1 - i];
    }

    std::cout << "Sum of main diagonal elements: " << mainDiagonalSum << std::endl;
    std::cout << "Sum of secondary diagonal elements: " << secondaryDiagonalSum << std::endl;
}

/**
 * @brief swaps two rows in a matrix
 * @param matrix the matrix to modify
 * @param row1 index of the first row
 * @param row2 index of the second row
 */
void swapRows(Matrix &matrix, int row1, int row2)
{
    if (matrix.empty())
    {
        std::cerr << "Error: Cannot swap rows in an empty matrix" << std::endl;
        return;
    }
    int n = matrix.size();
    if (row1 < 0 || row1 >= n || row2 < 0 || row2 >= n)
    {
        std::cerr << "Error: Row index out of bounds (" << row1 << ", " << row2 << "). Valid range is 0 to " << n - 1 << std::endl;
        return;
    }

    if (row1 == row2)
        return; // don't swap if the indices are the same

    // std::vector supports efficient swapping
    std::swap(matrix[row1], matrix[row2]);
}

/**
 * @brief swaps two columns in a matrix
 * @param matrix the matrix to modify
 * @param col1 index of the first column
 * @param col2 index of the second column
 */
void swapCols(Matrix &matrix, int col1, int col2)
{
    if (matrix.empty() || matrix[0].empty())
    {
        std::cerr << "Error: Cannot swap columns in an empty matrix" << std::endl;
        return;
    }
    int n = matrix.size();
    int m = matrix[0].size();

    if (col1 < 0 || col1 >= m || col2 < 0 || col2 >= m)
    {
        std::cerr << "Error: Column index out of bounds (" << col1 << ", " << col2 << "). Valid range is 0 to " << m - 1 << std::endl;
        return;
    }

    if (col1 == col2)
        return; // don't swap

    for (int i = 0; i < n; ++i)
    { // Iterate through rows
        std::swap(matrix[i][col1], matrix[i][col2]);
    }
}

/**
 * @brief updates a single element in the matrix
 * @param matrix the matrix to modify
 * @param row row index of the element
 * @param col column index of the element
 * @param newValue the new value for the element
 */
void updateElement(Matrix &matrix, int row, int col, int newValue)
{
    if (matrix.empty() || matrix[0].empty())
    {
        std::cerr << "Error: Cannot update element in an empty matrix" << std::endl;
        return;
    }
    int n = matrix.size();
    int m = matrix[0].size();

    if (row < 0 || row >= n || col < 0 || col >= m)
    {
        std::cerr << "Error: Index (" << row << ", " << col << ") out of bounds. Valid row range 0 to "
                  << n - 1 << ", valid col range 0 to " << m - 1 << std::endl;
        return;
    }

    matrix[row][col] = newValue;
}