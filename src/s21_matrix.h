#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define UNCOR_MATR 1
#define CALC_ERROR 2

#define SUCCESS 1
#define FAILURE 0
#define EPSILON 1e-7

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);  // Сравнение матриц
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);     // +
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);     // -
int s21_mult_number(matrix_t *A, double number, matrix_t *result);  // * on num
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);  // * on matrix
int s21_transpose(matrix_t *A, matrix_t *result);  //  Транспонирование матрицы
int s21_calc_complements(
    matrix_t *A,
    matrix_t *result);  // Минор матрицы и матрица алгебраических дополнений
int s21_determinant(matrix_t *A, double *result);  // Determinant
int s21_inverse_matrix(matrix_t *A, matrix_t *result);  // Обратная матрица
int s21_minor_matrix(matrix_t *A, matrix_t *minor, int row_i,
                     int column_j);  // доп минор

#endif  // S21_MATRIX_H
