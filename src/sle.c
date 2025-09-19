#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 1e-6

// Выделение памяти для квадратной матрицы
double **alloc_matrix(int rows, int columns) {
    double **matrix = malloc(rows * sizeof(double *) + rows * columns * sizeof(double));
    if (!matrix) return NULL;
    double *data = (double *)(matrix + rows);
    for (int i = 0; i < rows; i++) {
        matrix[i] = data + i * columns;
    }
    return matrix;
}

// Освобождение памяти матрицы
void free_matrix(double **matrix) {
    if (matrix) free(matrix);
}

// Копирование матрицы src в dst
void copy_matrix(double **src, double **dst, int rows, int columns) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++) dst[i][j] = src[i][j];
}

// Поиск определителя методом Гаусса с частичным выбором опорного элемента
double determinant(double **matrix, int size, int *error) {
    double det = 1.0;
    double sign = 1.0;
    double **temp = alloc_matrix(size, size);
    if (!temp) {
        *error = 1;
        return 0.0;
    }
    copy_matrix(matrix, temp, size, size);

    for (int i = 0; i < size; i++) {
        // Поиск максимального по модулю опорного элемента
        int pivot = i;
        for (int r = i + 1; r < size; r++) {
            if (fabs(temp[r][i]) > fabs(temp[pivot][i])) {
                pivot = r;
            }
        }
        if (fabs(temp[pivot][i]) < EPSILON) {
            det = 0.0;
            break;  // нулевой столбец — определитель 0
        }
        if (pivot != i) {
            double *tmp_row = temp[i];
            temp[i] = temp[pivot];
            temp[pivot] = tmp_row;
            sign = -sign;
        }
        double diag = temp[i][i];
        det *= diag;
        for (int r = i + 1; r < size; r++) {
            double coef = temp[r][i] / diag;
            for (int c = i; c < size; c++) {
                temp[r][c] -= coef * temp[i][c];
            }
        }
    }

    free_matrix(temp);
    return sign * det;
}

// Извлечение столбца из матрицы
double *extract_column(double **matrix, int rows, int column, int *error) {
    double *col = malloc(rows * sizeof(double));
    if (!col) {
        *error = 1;
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        col[i] = matrix[i][column];
    }
    return col;
}

// Замена столбца матрицы
void replace_column(double **matrix, const double *column, int rows, int col_index) {
    for (int i = 0; i < rows; i++) {
        matrix[i][col_index] = column[i];
    }
}

// Ввод расширенной матрицы (n × (n+1))
int input(double ***matrix, int *rows, int *columns) {
    if (scanf("%d %d", rows, columns) != 2) return 1;
    if (*rows <= 0 || *columns != *rows + 1) return 1;

    *matrix = alloc_matrix(*rows, *columns);
    if (!*matrix) return 1;

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *columns; j++) {
            if (scanf("%lf", &((*matrix)[i][j])) != 1) {
                free_matrix(*matrix);
                *matrix = NULL;
                return 1;
            }
        }
    }
    return 0;
}

// Вывод корней решения
void output_roots(const double *roots, int n, int error) {
    if (error) {
        printf("n/a");
        return;
    }
    for (int i = 0; i < n; i++) {
        printf("%.6lf", roots[i]);
        if (i != n - 1) printf(" ");
    }
}

// Решение СЛАУ методом Крамера
void solve_sle(double **matrix, int rows, int columns, double **roots, int *error) {
    int n = rows;
    double **work_matrix = alloc_matrix(n, n);
    if (!work_matrix) {
        *error = 1;
        return;
    }
    copy_matrix(matrix, work_matrix, n, n);

    double *last_col = extract_column(matrix, n, columns - 1, error);
    if (*error) {
        free_matrix(work_matrix);
        return;
    }

    double det_main = determinant(work_matrix, n, error);
    if (*error || fabs(det_main) < EPSILON) {
        *error = 1;
        free(last_col);
        free_matrix(work_matrix);
        return;
    }

    *roots = malloc(n * sizeof(double));
    if (!*roots) {
        *error = 1;
        free(last_col);
        free_matrix(work_matrix);
        return;
    }

    for (int i = 0; i < n; i++) {
        replace_column(work_matrix, last_col, n, i);
        double det_i = determinant(work_matrix, n, error);
        if (*error) {
            free(last_col);
            free_matrix(work_matrix);
            return;
        }
        (*roots)[i] = det_i / det_main;
        copy_matrix(matrix, work_matrix, n, n);
    }

    free(last_col);
    free_matrix(work_matrix);
}

int main(void) {
    double **matrix = NULL;
    double *roots = NULL;
    int rows = 0, columns = 0;
    int error = 0;

    error = input(&matrix, &rows, &columns);
    if (!error) {
        solve_sle(matrix, rows, columns, &roots, &error);
    }

    output_roots(roots, rows, error);

    free_matrix(matrix);
    free(roots);

    return error ? 1 : 0;
}
