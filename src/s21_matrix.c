#include "s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0 || result == NULL) {
    return UNCOR_MATR;
  }
  result->rows = rows;
  result->columns = columns;
  result->matrix = (double **)malloc(rows * sizeof(double *));

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = (double *)malloc(columns * sizeof(double));
  }

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) result->matrix[i][j] = 0;

  return OK;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    // Освобождение памяти для каждой строки матрицы
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }

    free(A->matrix);  // Освобождаем память для массива указателей на строки
    A->matrix = NULL;  // Обнуляем указатель на матрицу
    A->columns = 0;    // Обнуляем количество столбцов
    A->rows = 0;       // Обнуляем количество строк
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  // Проверка на NULL и размеры матриц
  if (A == NULL || B == NULL || A->rows != B->rows ||
      A->columns != B->columns) {
    return FAILURE;
  }

  // Сравнение элементов матриц
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) {
        return FAILURE;  // Если найдено несоответствие, возвращаем FAILURE
      }
    }
  }

  return SUCCESS;  // Если все элементы равны, возвращаем SUCCESS
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  // Проверка на NULL для входных матриц и результата
  if (A == NULL || B == NULL || result == NULL) {
    return UNCOR_MATR;
    ;
  }

  // Проверка на совпадение размеров матриц A и B
  if (A->rows != B->rows || A->columns != B->columns) {
    return CALC_ERROR;
  }

  // Создание результирующей матрицы с теми же размерами, что и у A и B
  if (s21_create_matrix(A->rows, A->columns, result) != 0) {
    return UNCOR_MATR;
  }

  // Сложение элементов матриц A и B и запись результата в результирующую
  // матрицу
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }

  return 0;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  // Проверка входных данных на корректность
  if (A == NULL || B == NULL || result == NULL) {
    return UNCOR_MATR;
  }

  // Проверка размеров матриц
  if (A->rows != B->rows || A->columns != B->columns) {
    return CALC_ERROR;
  }

  // Создание результирующей матрицы
  int create_status = s21_create_matrix(A->rows, A->columns, result);
  if (create_status != OK) {
    return create_status;
  }

  // Выполнение вычитания
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      // Вычитаем соответствующие элементы матриц A и B
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }

  return OK;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  // Проверка входных данных на корректность
  if (A == NULL || result == NULL) {
    return UNCOR_MATR;
  }

  // Создание результирующей матрицы с размерами, что и матрица A
  s21_create_matrix(A->rows, A->columns, result);

  // Умножение каждого элемента матрицы A на заданное число
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      // Вычисляем произведение и сохраняем в результирующей матрице
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return OK;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  // Проверка входных данных на корректность
  if (A == NULL || B == NULL || result == NULL) {
    return UNCOR_MATR;
  }
  if (A->columns != B->rows) {
    return CALC_ERROR;
  }
  // Создание результирующей матрицы с правильными размерами размерами
  s21_create_matrix(A->rows, B->columns, result);

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return OK;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  // Проверка входных данных на корректность
  if (A == NULL || result == NULL) {
    return UNCOR_MATR;
  }

  int create_status = s21_create_matrix(A->columns, A->rows, result);
  if (create_status != OK) {
    return create_status;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] =
          A->matrix[i][j];  // Обратите внимание на порядок индексов
    }
  }
  return OK;
}

// zipper head 4king  function

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  // Проверка входных данных на корректность
  if (A == NULL || result == NULL || A->rows != A->columns) {
    return UNCOR_MATR;
  }

  // Создание результирующей матрицы с теми же размерами, что и A
  s21_create_matrix(A->rows, A->columns, result);

  matrix_t minor;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      // Получаем минор для элемента A[i][j]
      s21_minor_matrix(A, &minor, i, j);

      // Вычисляем детерминант минора
      double det;
      s21_determinant(&minor, &det);
      result->matrix[i][j] =
          ((i + j) % 2 == 0 ? det : -det);  // Алгебраическое дополнение

      s21_remove_matrix(&minor);
    }
  }

  return OK;
}

int s21_determinant(matrix_t *A, double *result) {
  // Проверка входных данных на корректность
  if (A == NULL || result == NULL) {
    return UNCOR_MATR;
  }

  if (A->rows != A->columns) {
    return CALC_ERROR;
  }

  // Создаем копию матрицы для работы
  matrix_t temp;
  s21_create_matrix(A->rows, A->columns, &temp);

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      temp.matrix[i][j] = A->matrix[i][j];
    }
  }

  double det = 1.0;  // Начальное значение детерминанта

  for (int i = 0; i < temp.rows; i++) {
    // Проверка на нулевой элемент и обмен с нижележащей строкой
    if (temp.matrix[i][i] == 0) {
      int swap_row = i + 1;
      while (swap_row < temp.rows && temp.matrix[swap_row][i] == 0) {
        swap_row++;
      }
      if (swap_row == temp.rows) {
        *result = 0;  // Если не нашли ненулевой элемент, детерминант равен нулю
        s21_remove_matrix(&temp);
        return OK;
      }
      // Меняем строки
      for (int k = 0; k < temp.columns; k++) {
        double tmp = temp.matrix[i][k];
        temp.matrix[i][k] = temp.matrix[swap_row][k];
        temp.matrix[swap_row][k] = tmp;
      }
      det *= -1;  // Изменение порядка строк меняет знак детерминанта
    }

    // Приведение к верхнетреугольному виду
    for (int k = i + 1; k < temp.rows; k++) {
      double factor = temp.matrix[k][i] / temp.matrix[i][i];
      for (int j = i; j < temp.columns; j++) {
        temp.matrix[k][j] -= factor * temp.matrix[i][j];
      }
    }
  }

  // Вычисление детерминанта как произведение диагональных элементов
  for (int i = 0; i < temp.rows; i++) {
    det *= temp.matrix[i][i];
  }

  *result = det;
  s21_remove_matrix(&temp);

  return OK;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  // Проверка входных данных на корректность
  if (A == NULL || result == NULL) {
    return UNCOR_MATR;
  }

  if (A->rows != A->columns) {
    return CALC_ERROR;
  }

  double det;
  int det_status = s21_determinant(A, &det);

  if (det_status != OK || det == 0) {
    return CALC_ERROR;  // Если детерминант равен нулю, обратная матрица не
                        // существует
  }

  matrix_t complements;

  int comp_status = s21_calc_complements(A, &complements);

  if (comp_status != OK) {
    return comp_status;  // Ошибка при вычислении алгебраических дополнений
  }

  // Создание результирующей матрицы с размерами A
  int create_status = s21_create_matrix(A->rows, A->columns, result);

  if (create_status != OK) {
    s21_remove_matrix(&complements);
    return create_status;
  }

  for (int i = 0; i < complements.rows; i++) {
    for (int j = 0; j < complements.columns; j++) {
      result->matrix[i][j] = complements.matrix[j][i] /
                             det;  // Транспонирование и деление на детерминант
    }
  }

  s21_remove_matrix(&complements);

  return OK;
}

// Dop_Fun

int s21_minor_matrix(matrix_t *A, matrix_t *minor, int row_i, int column_j) {
  // Проверка входных данных
  if (A == NULL || minor == NULL || row_i < 0 || row_i >= A->rows ||
      column_j < 0 || column_j >= A->columns) {
    return UNCOR_MATR;  // Некорректные входные данные
  }

  // Создание минорной матрицы
  int create_status = s21_create_matrix(A->rows - 1, A->columns - 1, minor);
  if (create_status != OK) {
    return create_status;  // Ошибка при создании минорной матрицы
  }

  int n = 0;  // Индекс для строк в миноре
  for (int i = 0; i < A->rows; i++) {
    if (i == row_i) continue;  // Пропускаем удаляемую строку

    int k = 0;  // Индекс для столбцов в миноре
    for (int j = 0; j < A->columns; j++) {
      if (j == column_j) continue;  // Пропускаем удаляемый столбец

      minor->matrix[n][k] = A->matrix[i][j];  // Копируем элемент
      k++;
    }
    n++;  // Переход к следующей строке в миноре
  }

  return OK;  // Успешное завершение
}
