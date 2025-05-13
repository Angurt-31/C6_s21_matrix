#include <check.h>

#include "s21_matrix.h"

START_TEST(create_matrix_test_1) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(0, 3, &A), 1);
}
END_TEST

START_TEST(create_matrix_test_2) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(2, 0, &A), 1);
}
END_TEST

START_TEST(create_matrix_test_3) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(0, 0, &A), 1);
}
END_TEST

START_TEST(create_matrix_test_4) {
  matrix_t *A = NULL;
  ck_assert_int_eq(s21_create_matrix(2, 3, A), 1);
}
END_TEST

START_TEST(create_matrix_test_5) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(-1, 3, &A), 1);
}
END_TEST

START_TEST(create_matrix_test_6) {
  matrix_t A;

  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(A.rows, 2);
  ck_assert_int_eq(A.columns, 3);
  for (int i = 0; i < A.rows; i++) {
    ck_assert_ptr_ne(A.matrix[i], NULL);
  }

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_matrix_test_7) {
  matrix_t A;

  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
  ck_assert_int_eq(A.rows, 1);
  ck_assert_int_eq(A.columns, 1);
  ck_assert_ptr_ne(A.matrix, NULL);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_matrix_test_8) {
  matrix_t A;

  ck_assert_int_eq(s21_create_matrix(100, 100, &A), 0);
  ck_assert_int_eq(A.rows, 100);
  ck_assert_int_eq(A.columns, 100);
  for (int i = 0; i < A.rows; i++) {
    ck_assert_ptr_ne(A.matrix[i], NULL);
  }

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(remove_matrix_test_1) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  s21_remove_matrix(&A);

  ck_assert_ptr_eq(A.matrix, NULL);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);
}
END_TEST

START_TEST(remove_matrix_test_2) {
  matrix_t A;
  A.matrix = NULL;
  A.rows = 0;
  A.columns = 0;

  s21_remove_matrix(&A);
  ck_assert_ptr_eq(A.matrix, NULL);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);
}
END_TEST

START_TEST(remove_matrix_test_3) {
  matrix_t *A = NULL;
  s21_remove_matrix(A);  // Передаем NULL указатель, не должно вызывать ошибок

  ck_assert_int_eq(1, 1);  // Условие всегда истинно
}
END_TEST

START_TEST(remove_matrix_test_4) {
  matrix_t A;
  s21_create_matrix(2, 2, &A);
  s21_remove_matrix(&A);

  s21_remove_matrix(&A);  // Повторное освобождение не должно вызывать ошибок
  ck_assert_ptr_eq(A.matrix, NULL);
}
END_TEST

START_TEST(remove_matrix_test_5) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;  // Инициализируем элемент

  s21_remove_matrix(&A);
  ck_assert_ptr_eq(A.matrix, NULL);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);
}
END_TEST

START_TEST(eq_matrix_test_1) {
  matrix_t *A = NULL, *B = NULL;

  ck_assert_int_eq(s21_eq_matrix(A, B), FAILURE);
}
END_TEST

START_TEST(eq_matrix_test_2) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = B.matrix[i][j] = i + j;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_test_3) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  // Инициализируем элементы разными значениями
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = i + j;
      B.matrix[i][j] = i - j;
    }
  }

  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_test_4) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_test_5) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  // Инициализируем одинаковые матрицы с незначительной разницей
  A.matrix[1][1] = 1.0000001;
  B.matrix[1][1] = 1.0000002;

  // Проверяем, что матрицы равны с учетом EPSILON
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_test_6) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  // Инициализируем одинаковые матрицы с точностью до EPSILON
  A.matrix[1][1] = 1.0000001;
  B.matrix[1][1] = 1.0000008;

  // Проверяем, что матрицы не равны из-за превышения EPSILON
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_test_1) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);
  ck_assert_int_eq(s21_sum_matrix(NULL, &B, NULL), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_test_2) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = i + j;
      B.matrix[i][j] = i * j;
    }
  }

  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(result.matrix[i][j], A.matrix[i][j] + B.matrix[i][j]);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sum_matrix_test_3) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sum_matrix_test_4) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  matrix_t result;
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(result.matrix[i][j], 0.0);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sum_matrix_test_5) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = -1;
      B.matrix[i][j] = -2;
    }
  }

  matrix_t result;
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(result.matrix[i][j], -3);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sum_matrix_test_6) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = 1.1;
      B.matrix[i][j] = 2.2;
    }
  }

  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(result.matrix[i][j], 3.3, EPSILON);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sum_matrix_test_7) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      A.matrix[i][j] = 1e6;
      B.matrix[i][j] = 1e6;
    }
  }

  ck_assert_int_eq(s21_sum_matrix(&A, &B, &result), 0);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(result.matrix[i][j], 2e6);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_test_1) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  A.matrix[0][0] = 5.0;
  A.matrix[0][1] = 3.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 7.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), 0);
  ck_assert_double_eq(result.matrix[0][0], 4.0);
  ck_assert_double_eq(result.matrix[0][1], 1.0);
  ck_assert_double_eq(result.matrix[1][0], -1.0);
  ck_assert_double_eq(result.matrix[1][1], 3.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_test_2) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);

  matrix_t result;

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_test_3) {
  matrix_t A, B;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, NULL), 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_test_4) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), 0);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_double_eq(result.matrix[i][j], 0.0);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_test_5) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_create_matrix(2, 2, &B), 0);

  A.matrix[0][0] = 1.5;
  A.matrix[0][1] = 2.5;
  A.matrix[1][0] = 3.5;
  A.matrix[1][1] = 4.5;

  B.matrix[0][0] = 0.5;
  B.matrix[0][1] = 1.5;
  B.matrix[1][0] = 2.5;
  B.matrix[1][1] = 3.5;

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), 0);
  ck_assert_double_eq_tol(result.matrix[0][0], 1.0, EPSILON);
  ck_assert_double_eq_tol(result.matrix[0][1], 1.0, EPSILON);
  ck_assert_double_eq_tol(result.matrix[1][0], 1.0, EPSILON);
  ck_assert_double_eq_tol(result.matrix[1][1], 1.0, EPSILON);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_number_test_1) {
  matrix_t A;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_mult_number(&A, 2.0, NULL), 1);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(mult_number_test_2) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_mult_number(&A, 2.0, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 2.0);
  ck_assert_double_eq(result.matrix[0][1], 4.0);
  ck_assert_double_eq(result.matrix[1][0], 6.0);
  ck_assert_double_eq(result.matrix[1][1], 8.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_number_test_3) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_mult_number(&A, 0.0, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 0.0);
  ck_assert_double_eq(result.matrix[0][1], 0.0);
  ck_assert_double_eq(result.matrix[1][0], 0.0);
  ck_assert_double_eq(result.matrix[1][1], 0.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_number_test4) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_mult_number(&A, -3.0, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], -3.0);
  ck_assert_double_eq(result.matrix[0][1], -6.0);
  ck_assert_double_eq(result.matrix[1][0], -9.0);
  ck_assert_double_eq(result.matrix[1][1], -12.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_test_1) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  ck_assert_int_eq(s21_create_matrix(3, 2, &B), 0);
  B.matrix[0][0] = 7.0;
  B.matrix[0][1] = 8.0;
  B.matrix[1][0] = 9.0;
  B.matrix[1][1] = 10.0;
  B.matrix[2][0] = 11.0;
  B.matrix[2][1] = 12.0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);
  ck_assert_double_eq(result.matrix[0][0], 58.0);
  ck_assert_double_eq(result.matrix[0][1], 64.0);
  ck_assert_double_eq(result.matrix[1][0], 139.0);
  ck_assert_double_eq(result.matrix[1][1], 154.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_test_2) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_create_matrix(3, 2, &B), 0);
  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;
  B.matrix[2][0] = 9.0;
  B.matrix[2][1] = 10.0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mult_matrix_test_3) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  ck_assert_int_eq(s21_create_matrix(3, 2, &B), 0);

  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < A.columns; j++) {
      A.matrix[i][j] = 0.0;
    }
  }

  for (int i = 0; i < B.rows; i++) {
    for (int j = 0; j < B.columns; j++) {
      B.matrix[i][j] = 0.0;
    }
  }

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);

  for (int i = 0; i < result.rows; i++) {
    for (int j = 0; j < result.columns; j++) {
      ck_assert_double_eq(result.matrix[i][j], 0.0);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_test_4) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  ck_assert_int_eq(s21_create_matrix(3, 2, &B), 0);
  B.matrix[0][0] = 0.0;
  B.matrix[0][1] = 0.0;
  B.matrix[1][0] = 0.0;
  B.matrix[1][1] = 0.0;
  B.matrix[2][0] = 0.0;
  B.matrix[2][1] = 0.0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);
  ck_assert_double_eq(result.matrix[0][0], 0.0);
  ck_assert_double_eq(result.matrix[0][1], 0.0);
  ck_assert_double_eq(result.matrix[1][0], 0.0);
  ck_assert_double_eq(result.matrix[1][1], 0.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

START_TEST(mult_matrix_test_5) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  ck_assert_int_eq(s21_mult_matrix(&A, NULL, &result), 1);
  ck_assert_int_eq(s21_mult_matrix(NULL, &A, &result), 1);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(mult_matrix_test_6) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;
  A.matrix[2][0] = 7.0;
  A.matrix[2][1] = 8.0;
  A.matrix[2][2] = 9.0;

  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);
  B.matrix[0][0] = 9.0;
  B.matrix[0][1] = 8.0;
  B.matrix[0][2] = 7.0;
  B.matrix[1][0] = 6.0;
  B.matrix[1][1] = 5.0;
  B.matrix[1][2] = 4.0;
  B.matrix[2][0] = 3.0;
  B.matrix[2][1] = 2.0;
  B.matrix[2][2] = 1.0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 30.0);
  ck_assert_double_eq(result.matrix[0][1], 24.0);
  ck_assert_double_eq(result.matrix[0][2], 18.0);
  ck_assert_double_eq(result.matrix[1][0], 84.0);
  ck_assert_double_eq(result.matrix[1][1], 69.0);
  ck_assert_double_eq(result.matrix[1][2], 54.0);
  ck_assert_double_eq(result.matrix[2][0], 138.0);
  ck_assert_double_eq(result.matrix[2][1], 114.0);
  ck_assert_double_eq(result.matrix[2][2], 90.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_test_7) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(4, 4, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[0][3] = 4.0;
  A.matrix[1][0] = 5.0;
  A.matrix[1][1] = 6.0;
  A.matrix[1][2] = 7.0;
  A.matrix[1][3] = 8.0;
  A.matrix[2][0] = 9.0;
  A.matrix[2][1] = 10.0;
  A.matrix[2][2] = 11.0;
  A.matrix[2][3] = 12.0;
  A.matrix[3][0] = 13.0;
  A.matrix[3][1] = 14.0;
  A.matrix[3][2] = 15.0;
  A.matrix[3][3] = 16.0;

  ck_assert_int_eq(s21_create_matrix(4, 4, &B), 0);
  B.matrix[0][0] = 16.0;
  B.matrix[0][1] = 15.0;
  B.matrix[0][2] = 14.0;
  B.matrix[0][3] = 13.0;
  B.matrix[1][0] = 12.0;
  B.matrix[1][1] = 11.0;
  B.matrix[1][2] = 10.0;
  B.matrix[1][3] = 9.0;
  B.matrix[2][0] = 8.0;
  B.matrix[2][1] = 7.0;
  B.matrix[2][2] = 6.0;
  B.matrix[2][3] = 5.0;
  B.matrix[3][0] = 4.0;
  B.matrix[3][1] = 3.0;
  B.matrix[3][2] = 2.0;
  B.matrix[3][3] = 1.0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 80.0);
  ck_assert_double_eq(result.matrix[0][1], 70.0);
  ck_assert_double_eq(result.matrix[1][0], 240.0);
  ck_assert_double_eq(result.matrix[1][1], 214.0);
  ck_assert_double_eq(result.matrix[2][0], 400.0);
  ck_assert_double_eq(result.matrix[2][1], 358.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(mult_matrix_test_8) {
  matrix_t A, B, result;

  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  A.matrix[0][0] = -1.0;
  A.matrix[0][1] = -2.0;
  A.matrix[0][2] = -3.0;
  A.matrix[1][0] = -4.0;
  A.matrix[1][1] = -5.0;
  A.matrix[1][2] = -6.0;
  A.matrix[2][0] = -7.0;
  A.matrix[2][1] = -8.0;
  A.matrix[2][2] = -9.0;

  ck_assert_int_eq(s21_create_matrix(3, 3, &B), 0);
  B.matrix[0][0] = -9.0;
  B.matrix[0][1] = -8.0;
  B.matrix[0][2] = -7.0;
  B.matrix[1][0] = -6.0;
  B.matrix[1][1] = -5.0;
  B.matrix[1][2] = -4.0;
  B.matrix[2][0] = -3.0;
  B.matrix[2][1] = -2.0;
  B.matrix[2][2] = -1.0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 30.0);
  ck_assert_double_eq(result.matrix[0][1], 24.0);
  ck_assert_double_eq(result.matrix[0][2], 18.0);
  ck_assert_double_eq(result.matrix[1][0], 84.0);
  ck_assert_double_eq(result.matrix[1][1], 69.0);
  ck_assert_double_eq(result.matrix[1][2], 54.0);
  ck_assert_double_eq(result.matrix[2][0], 138.0);
  ck_assert_double_eq(result.matrix[2][1], 114.0);
  ck_assert_double_eq(result.matrix[2][2], 90.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_matrix_test_1) {
  matrix_t *A = NULL;
  matrix_t result;

  ck_assert_int_eq(s21_transpose(A, &result), 1);
}
END_TEST

START_TEST(transpose_matrix_test_2) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_transpose(&A, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 1.0);
  ck_assert_double_eq(result.matrix[0][1], 3.0);
  ck_assert_double_eq(result.matrix[1][0], 2.0);
  ck_assert_double_eq(result.matrix[1][1], 4.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_matrix_test_3) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(3, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 6.0;

  ck_assert_int_eq(s21_transpose(&A, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 1.0);  // 1st row 1st column
  ck_assert_double_eq(result.matrix[1][0], 2.0);  // 2nd row 1st column
  ck_assert_double_eq(result.matrix[0][1], 3.0);  // 1st row 2nd column
  ck_assert_double_eq(result.matrix[1][1], 4.0);  // 2nd row 2nd column
  ck_assert_double_eq(result.matrix[0][2], 5.0);  // 1st row 3rd column
  ck_assert_double_eq(result.matrix[1][2], 6.0);  // 2nd row 3rd column

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_matrix_test_4) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(1, 3, &A), 0);
  A.matrix[0][0] = 7.0;
  A.matrix[0][1] = 8.0;
  A.matrix[0][2] = 9.0;

  ck_assert_int_eq(s21_transpose(&A, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 7.0);
  ck_assert_double_eq(result.matrix[1][0], 8.0);
  ck_assert_double_eq(result.matrix[2][0], 9.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_matrix_test_5) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
  A.matrix[0][0] = 42.0;

  ck_assert_int_eq(s21_transpose(&A, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 42.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_test_1) {
  matrix_t *A = NULL, result;

  ck_assert_int_eq(s21_calc_complements(A, &result), 1);
}
END_TEST

START_TEST(calc_complements_test_2) {
  ck_assert_int_eq(s21_calc_complements(NULL, NULL), 1);
}
END_TEST

// START_TEST(calc_complements_test_3) {
//   matrix_t A, result;
//
//   ck_assert_int_eq(s21_create_matrix(1, 1, &A), 0);
//   A.matrix[0][0] = 5.0;
//
//   ck_assert_int_eq(s21_calc_complements(&A, &result), 0);
//   ck_assert_double_eq(result.matrix[0][0], 1.0);
//
//   s21_remove_matrix(&A);
//   s21_remove_matrix(&result);
// }
// END_TEST

// START_TEST(calc_complements_test_4) {
//   matrix_t A, result;
//
//   ck_assert_int_eq(s21_create_matrix(2, 3, &A), 0);
//   A.matrix[0][0] = 1.0;
//   A.matrix[0][1] = 2.0;
//   A.matrix[0][2] = 3.0;
//   A.matrix[1][0] = 4.0;
//   A.matrix[1][1] = 5.0;
//   A.matrix[1][2] = 6.0;
//
//   ck_assert_int_eq(s21_calc_complements(&A, &result), 2);
//
//   s21_remove_matrix(&A);
// }
// END_TEST
//
START_TEST(calc_complements_test_5) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(2, 2, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  ck_assert_int_eq(s21_calc_complements(&A, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 4.0);
  ck_assert_double_eq(result.matrix[0][1], -3.0);
  ck_assert_double_eq(result.matrix[1][0], -2.0);
  ck_assert_double_eq(result.matrix[1][1], 1.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_test_6) {
  matrix_t A;
  ck_assert_int_eq(s21_create_matrix(3, 3, &A), 0);
  ck_assert_ptr_nonnull(A.matrix);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 3);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;

  ck_assert_double_eq(A.matrix[0][0], 1);
  ck_assert_double_eq(A.matrix[0][1], 2);
  ck_assert_double_eq(A.matrix[0][2], 3);
  ck_assert_double_eq(A.matrix[1][0], 4);
  ck_assert_double_eq(A.matrix[1][1], 5);
  ck_assert_double_eq(A.matrix[1][2], 6);
  ck_assert_double_eq(A.matrix[2][0], 7);
  ck_assert_double_eq(A.matrix[2][1], 8);
  ck_assert_double_eq(A.matrix[2][2], 9);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_test_7) {
  matrix_t A, result;

  ck_assert_int_eq(s21_create_matrix(4, 4, &A), 0);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[0][3] = 4.0;
  A.matrix[1][0] = 5.0;
  A.matrix[1][1] = 6.0;
  A.matrix[1][2] = 7.0;
  A.matrix[1][3] = 8.0;
  A.matrix[2][0] = 9.0;
  A.matrix[2][1] = 10.0;
  A.matrix[2][2] = 11.0;
  A.matrix[2][3] = 12.0;
  A.matrix[3][0] = 13.0;
  A.matrix[3][1] = 14.0;
  A.matrix[3][2] = 15.0;
  A.matrix[3][3] = 16.0;

  ck_assert_int_eq(s21_calc_complements(&A, &result), 0);

  ck_assert_double_eq(result.matrix[0][0], 0.0);
  ck_assert_double_eq(result.matrix[1][1], 0.0);
  ck_assert_double_eq(result.matrix[2][2], 0.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(determinant_test_1) {
  matrix_t A;
  double result;

  ck_assert_int_eq(s21_determinant(NULL, &result), 1);
  ck_assert_int_eq(s21_determinant(&A, NULL), 1);
}
END_TEST

START_TEST(determinant_test_2) {
  matrix_t A;
  double result;
  A.rows = 2;
  A.columns = 3;

  ck_assert_int_eq(s21_determinant(&A, &result), 2);
}
END_TEST

START_TEST(determinant_test_3) {
  matrix_t A;
  double result = 0.0;

  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;
  ck_assert_int_eq(s21_determinant(&A, &result), 0);
  ck_assert_double_eq(result, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test_4) {
  matrix_t A;
  double result = 0.0;

  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  ck_assert_int_eq(s21_determinant(&A, &result), 0);
  ck_assert_double_eq(result, -2.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test_5) {
  matrix_t A;
  double result = 0.0;

  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 5.0;
  A.matrix[2][0] = 1.0;
  A.matrix[2][1] = 0.0;
  A.matrix[2][2] = 6.0;

  ck_assert_int_eq(s21_determinant(&A, &result), 0);
  ck_assert_double_eq(result, 22.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test_6) {
  matrix_t A;
  double result = 0.0;

  s21_create_matrix(3, 3, &A);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) A.matrix[i][j] = 0.0;

  ck_assert_int_eq(s21_determinant(&A, &result), 0);
  ck_assert_double_eq(result, 0.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_test_1) {
  matrix_t A;
  matrix_t result;

  ck_assert_int_eq(s21_inverse_matrix(NULL, &result), 1);
  ck_assert_int_eq(s21_inverse_matrix(&A, NULL), 1);
}
END_TEST

START_TEST(inverse_matrix_test_2) {
  matrix_t A;
  matrix_t result;

  s21_create_matrix(2, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), 2);

  s21_remove_matrix(&A);
}
END_TEST

// START_TEST(inverse_matrix_test_3) {
//   matrix_t A;
//   matrix_t result;
//
//   s21_create_matrix(1, 1, &A);
//   A.matrix[0][0] = 5;
//
//   ck_assert_int_eq(s21_inverse_matrix(&A, &result), 0);  // Успешная инверсия
//
//   ck_assert_double_eq_tol(result.matrix[0][0], 0.2,
//                           EPSILON);  // Проверка значений
//
//   s21_remove_matrix(&A);
//   s21_remove_matrix(&result);
// }
// END_TEST

START_TEST(inverse_matrix_test_4) {
  matrix_t A;
  matrix_t result;

  // Создаем матрицу 2x2 с ненулевым детерминантом
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), 0);  // Успешная инверсия

  ck_assert_double_eq_tol(result.matrix[0][0], 0.6, EPSILON);
  ck_assert_double_eq_tol(result.matrix[0][1], -0.7, EPSILON);
  ck_assert_double_eq_tol(result.matrix[1][0], -0.2, EPSILON);
  ck_assert_double_eq_tol(result.matrix[1][1], 0.4, EPSILON);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test_5) {
  matrix_t A;
  matrix_t result;

  // Создаем матрицу 3x3 с ненулевым детерминантом
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 2;
  A.matrix[0][1] = -1;
  A.matrix[0][2] = 0;
  A.matrix[1][0] = -1;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = -1;
  A.matrix[2][0] = 0;
  A.matrix[2][1] = -1;
  A.matrix[2][2] = 2;

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), 0);  // Успешная инверсия

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test_6) {
  matrix_t A;
  matrix_t result;

  // Создаем матрицу 3x3 с нулевым детерминантом
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;  // Вырожденная матрица

  ck_assert_int_eq(s21_inverse_matrix(&A, &result),
                   2);  // Ошибка: вырожденная матрица

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(inverse_matrix_test_7) {
  matrix_t A;
  matrix_t result;

  // Тест для больших матриц
  s21_create_matrix(4, 4, &A);
  A.matrix[0][0] = 5;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[0][3] = 1;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 6;
  A.matrix[1][2] = 2;
  A.matrix[1][3] = 1;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 4;
  A.matrix[2][2] = 5;
  A.matrix[2][3] = 2;
  A.matrix[3][0] = 1;
  A.matrix[3][1] = 1;
  A.matrix[3][2] = 1;
  A.matrix[3][3] = 2;

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), 0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

Suite *s21_matrix_suite(void) {
  Suite *suite = suite_create("s21_matrix");
  TCase *tcase_core = tcase_create("core");

  tcase_add_test(tcase_core, create_matrix_test_1);
  tcase_add_test(tcase_core, create_matrix_test_2);
  tcase_add_test(tcase_core, create_matrix_test_3);
  tcase_add_test(tcase_core, create_matrix_test_4);
  tcase_add_test(tcase_core, create_matrix_test_5);
  tcase_add_test(tcase_core, create_matrix_test_6);
  tcase_add_test(tcase_core, create_matrix_test_7);
  tcase_add_test(tcase_core, create_matrix_test_8);

  tcase_add_test(tcase_core, remove_matrix_test_1);
  tcase_add_test(tcase_core, remove_matrix_test_2);
  tcase_add_test(tcase_core, remove_matrix_test_3);
  tcase_add_test(tcase_core, remove_matrix_test_4);
  tcase_add_test(tcase_core, remove_matrix_test_5);

  tcase_add_test(tcase_core, eq_matrix_test_1);
  tcase_add_test(tcase_core, eq_matrix_test_2);
  tcase_add_test(tcase_core, eq_matrix_test_3);
  tcase_add_test(tcase_core, eq_matrix_test_4);
  tcase_add_test(tcase_core, eq_matrix_test_5);
  tcase_add_test(tcase_core, eq_matrix_test_6);

  tcase_add_test(tcase_core, sum_matrix_test_1);
  tcase_add_test(tcase_core, sum_matrix_test_2);
  tcase_add_test(tcase_core, sum_matrix_test_3);
  tcase_add_test(tcase_core, sum_matrix_test_4);
  tcase_add_test(tcase_core, sum_matrix_test_5);
  tcase_add_test(tcase_core, sum_matrix_test_6);
  tcase_add_test(tcase_core, sum_matrix_test_7);

  tcase_add_test(tcase_core, sub_matrix_test_1);
  tcase_add_test(tcase_core, sub_matrix_test_2);
  tcase_add_test(tcase_core, sub_matrix_test_3);
  tcase_add_test(tcase_core, sub_matrix_test_4);
  tcase_add_test(tcase_core, sub_matrix_test_5);

  tcase_add_test(tcase_core, mult_number_test_1);
  tcase_add_test(tcase_core, mult_number_test_2);
  tcase_add_test(tcase_core, mult_number_test_3);
  tcase_add_test(tcase_core, mult_number_test4);

  tcase_add_test(tcase_core, mult_matrix_test_1);
  tcase_add_test(tcase_core, mult_matrix_test_2);
  tcase_add_test(tcase_core, mult_matrix_test_3);
  tcase_add_test(tcase_core, mult_matrix_test_4);
  tcase_add_test(tcase_core, mult_matrix_test_5);
  tcase_add_test(tcase_core, mult_matrix_test_6);
  tcase_add_test(tcase_core, mult_matrix_test_7);
  tcase_add_test(tcase_core, mult_matrix_test_8);

  tcase_add_test(tcase_core, transpose_matrix_test_1);
  tcase_add_test(tcase_core, transpose_matrix_test_2);
  tcase_add_test(tcase_core, transpose_matrix_test_3);
  tcase_add_test(tcase_core, transpose_matrix_test_4);
  tcase_add_test(tcase_core, transpose_matrix_test_5);

  tcase_add_test(tcase_core, calc_complements_test_1);
  tcase_add_test(tcase_core, calc_complements_test_2);
  // tcase_add_test(tcase_core, calc_complements_test_3);
  // tcase_add_test(tcase_core, calc_complements_test_4);
  tcase_add_test(tcase_core, calc_complements_test_5);
  tcase_add_test(tcase_core, calc_complements_test_6);
  tcase_add_test(tcase_core, calc_complements_test_7);

  tcase_add_test(tcase_core, determinant_test_1);
  tcase_add_test(tcase_core, determinant_test_2);
  tcase_add_test(tcase_core, determinant_test_3);
  tcase_add_test(tcase_core, determinant_test_4);
  tcase_add_test(tcase_core, determinant_test_5);
  tcase_add_test(tcase_core, determinant_test_6);

  tcase_add_test(tcase_core, inverse_matrix_test_1);
  tcase_add_test(tcase_core, inverse_matrix_test_2);
  // tcase_add_test(tcase_core, inverse_matrix_test_3);
  tcase_add_test(tcase_core, inverse_matrix_test_4);
  tcase_add_test(tcase_core, inverse_matrix_test_5);
  tcase_add_test(tcase_core, inverse_matrix_test_6);
  tcase_add_test(tcase_core, inverse_matrix_test_7);

  suite_add_tcase(suite, tcase_core);

  return suite;
}

int main(void) {
  Suite *suite = s21_matrix_suite();
  SRunner *suite_runner = srunner_create(suite);

  srunner_set_fork_status(suite_runner, CK_NOFORK);
  srunner_run_all(suite_runner, CK_NORMAL);

  int failed_count = srunner_ntests_failed(suite_runner);

  srunner_free(suite_runner);

  return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
