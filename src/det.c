#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define EPSILON 1e-6

// Аллокация матрицы квадратной (единственный блок памяти)
double **alloc_matrix(int n) {
    double **matrix = malloc(n * sizeof(double *) + n * n * sizeof(double));
    if (!matrix) return NULL;
    double *data = (double *)(matrix + n);
    for (int i = 0; i < n; i++) matrix[i] = data + i * n;
    return matrix;
}
void free_matrix(double **matrix) { free(matrix); }

// Ввод матрицы с проверкой
int read_matrix(double ***matrix, int *n) {
    if (scanf("%d", n) != 1 || *n <= 0) return 1;
    int tmp;
    if (scanf("%d", &tmp) != 1 || tmp != *n) return 1;
    *matrix = alloc_matrix(*n);
    if (!*matrix) return 1;
    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *n; j++)
            if (scanf("%lf", &((*matrix)[i][j])) != 1) return 1;
    return 0;
}

// Гаусс — метод подсчета определителя
double det(double **matrix, int n) {
    double sign = 1., det_val = 1.;
    for (int i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && fabs(matrix[pivot][i]) < EPSILON) pivot++;
        if (pivot == n) return 0.0;
        if (pivot != i) {
            double *tmp = matrix[i];
            matrix[i] = matrix[pivot];
            matrix[pivot] = tmp;
            sign *= -1;
        }
        double diag = matrix[i][i];
        for (int j = i + 1; j < n; j++) {
            double coef = matrix[j][i] / diag;
            for (int k = i; k < n; k++) matrix[j][k] -= coef * matrix[i][k];
        }
        det_val *= diag;
    }
    return sign * det_val;
}

// Вывод результата
void print_result(double result, int error) {
    if (error)
        printf("n/a");
    else
        printf("%.6lf", result);
}

int main(void) {
    double **matrix = NULL;
    int n, error = read_matrix(&matrix, &n);
    double result = 0.0;
    if (!error) result = det(matrix, n);
    print_result(result, error);
    free_matrix(matrix);
    return 0;
}
