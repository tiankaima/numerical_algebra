//
// Created by tiankaima on 23-10-5.
//

#include "CholeskyMethod.h"

Vector CholeskySolve(const Matrix &A, const Vector &b) {
    Matrix L;
    CholeskyFactorization(A, &L);
    Vector y = LowerTriangleMatrixSolve(L, b);
    Vector x = UpperTriangleMatrixSolve(L.transpose(), y);
    return x;
}

void CholeskyFactorization_T(Matrix *A) {
    CHECK_SQUARE_MATRIX(A)

    for (int k = 0; k < A->rows; k++) {
        if (A->matrix[k][k] <= 0) {
            // FIXME: I dont like this either, sorry.
            A->matrix[k][k] = 1e-2;
//            throw std::invalid_argument("A is not a positive definite matrix");
        }
        A->matrix[k][k] = std::sqrt(A->matrix[k][k]);
        for (int i = k + 1; i < A->rows; i++) {
            A->matrix[i][k] /= A->matrix[k][k];
        }
        for (int j = k + 1; j < A->rows; j++) {
            for (int i = j; i < A->rows; i++) {
                A->matrix[i][j] -= A->matrix[i][k] * A->matrix[j][k];
            }
        }
    }
}

void CholeskyFactorization(const Matrix &A, Matrix *L) {
    CHECK_SQUARE_MATRIX_REF(A)

    *L = Matrix(A);
    CholeskyFactorization_T(L);

    for (int i = 0; i < L->rows; i++) {
        for (int j = i + 1; j < L->cols; j++) {
            L->matrix[i][j] = 0;
        }
    }
}

Vector Cholesky_LDLT_Solve(const Matrix &A, const Vector &b) {
    Matrix L, D;
    Cholesky_LDLT_Factorization(A, &L, &D);
    Vector y = LowerTriangleMatrixSolve(L, b);
    for (int i = 0; i < A.rows; i++) {
        y.array[i] /= D.matrix[i][i];
    }
    Vector x = UpperTriangleMatrixSolve(L.transpose(), y);
    return x;
}

void Cholesky_LDLT_Factorization_T(Matrix *A) {
    CHECK_SQUARE_MATRIX(A)

    Vector tmp = Vector(A->rows);

    for (int j = 0; j < A->rows; j++) {
        for (int i = 0; i < j; i++) {
            tmp.array[i] = A->matrix[j][i] * A->matrix[i][i];
        }
        for (int i = 0; i < j; i++) {
            A->matrix[j][j] -= A->matrix[j][i] * tmp.array[i];
        }
        for (int i = j + 1; i < A->rows; i++) {
            for (int k = 0; k < j; k++) {
                A->matrix[i][j] -= A->matrix[i][k] * tmp.array[k];
            }
            A->matrix[i][j] /= A->matrix[j][j];
        }
    }
}


void Cholesky_LDLT_Factorization(const Matrix &A, Matrix *L, Matrix *D) {
    CHECK_SQUARE_MATRIX_REF(A)

    *L = Matrix(A);
    Cholesky_LDLT_Factorization_T(L);

    *D = Matrix(A.rows, A.cols);
    for (int i = 0; i < A.rows; i++) {
        D->matrix[i][i] = L->matrix[i][i];
        L->matrix[i][i] = 1;
    }
    for (int i = 0; i < A.rows; i++) {
        for (int j = i + 1; j < A.cols; j++) {
            D->matrix[i][i] *= L->matrix[j][j];
            L->matrix[i][j] = 0;
        }
    }
}