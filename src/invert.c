#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define EPS 1e-8

// Единая аллокация N×N матрицы
double **alloc_matrix(int n) {
    double **a = malloc(n * sizeof(double *) + n * n * sizeof(double));
    if (!a) return NULL;
    double *data = (double *)(a + n);
    for (int i = 0; i < n; ++i) a[i] = data + i * n;
    return a;
}

void free_matrix(double **a) {
    if (a) free(a);
}

// Ввод квадратной матрицы
int input_matrix(double ***matrix, int *n) {
    int m;
    if (scanf("%d%d", n, &m) != 2 || *n <= 0 || m != *n) return 1;
    *matrix = alloc_matrix(*n);
    if (!*matrix) return 1;
    for (int i = 0; i < *n; ++i)
        for (int j = 0; j < *n; ++j)
            if (scanf("%lf", &((*matrix)[i][j])) != 1) return 1;
    return 0;
}

// Вывод матрицы — пробел между числами, без пробела в конце строки/файла
void print_matrix(double **a, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            printf("%.6lf", a[i][j]);
            if (j < n - 1)
                printf(" ");
            else if (i < n - 1)
                printf("\n");
        }
}

// Обратная матрица через прямой/обратный ход Гаусса
int invert_matrix(double **a, double **inv, int n) {
    // build augmented matrix
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) inv[i][j] = (i == j);

    for (int col = 0; col < n; ++col) {
        // поиск максимального по модулю для устойчивости
        int pivot = col;
        for (int i = col + 1; i < n; ++i)
            if (fabs(a[i][col]) > fabs(a[pivot][col])) pivot = i;
        if (fabs(a[pivot][col]) < EPS) return 1;  // необратима

        // swap строки
        if (pivot != col) {
            double *tmp = a[col];
            a[col] = a[pivot];
            a[pivot] = tmp;
            double *tmv = inv[col];
            inv[col] = inv[pivot];
            inv[pivot] = tmv;
        }

        double diag = a[col][col];

        // Привести ведущий коэффициент к 1
        for (int j = 0; j < n; ++j) a[col][j] /= diag, inv[col][j] /= diag;

        // Занулить остальные элементы в столбце
        for (int i = 0; i < n; ++i) {
            if (i == col) continue;
            double f = a[i][col];
            for (int j = 0; j < n; ++j) {
                a[i][j] -= f * a[col][j];
                inv[i][j] -= f * inv[col][j];
            }
        }
    }
    return 0;
}

int main(void) {
    int n, error = 0;
    double **matrix = NULL, **inverse = NULL;

    error = input_matrix(&matrix, &n);
    if (!error) inverse = alloc_matrix(n);
    if (!error && !inverse) error = 1;
    if (!error) {
        // Создаем копию, потому что в ходе обращения матрица изменится
        double **copy = alloc_matrix(n);
        if (!copy)
            error = 1;
        else {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j) copy[i][j] = matrix[i][j];
            error = invert_matrix(copy, inverse, n);
            free_matrix(copy);
        }
    }

    if (error)
        printf("n/a");
    else
        print_matrix(inverse, n);

    free_matrix(matrix);
    free_matrix(inverse);
    return 0;
}
