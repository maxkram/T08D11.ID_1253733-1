#include <stdio.h>
#include <stdlib.h>

void sort_vertical(int **matrix, int n, int m, int **result_matrix);
void sort_horizontal(int **matrix, int n, int m, int **result_matrix);
void input(int **matrix, int *n, int *m);
void output(int **matrix, int n, int m);

int main() {
    int **matrix = NULL, **result = NULL;
    int n, m;
    int error = 0;

    input(matrix, &n, &m);

    if (n <= 0 || m <= 0) {
        printf("n/a");
        return 0;
    }

    result = malloc(n * m * sizeof(int) + n * sizeof(int *));
    if (result == NULL) {
        printf("n/a");
        free(matrix);
        return 0;
    }
    int *data = (int *)(result + n);
    for (int i = 0; i < n; i++) {
        result[i] = data + m * i;
    }

    sort_vertical(matrix, n, m, result);
    output(result, n, m);
    printf("\n\n");

    sort_horizontal(matrix, n, m, result);
    output(result, n, m);

    free(matrix);
    free(result);

    return 0;
}

void input(int **matrix, int *n, int *m) {
    if (scanf("%d %d", n, m) != 2 || *n <= 0 || *m <= 0) {
        *n = 0;
        *m = 0;
        return;
    }

    matrix = malloc(*n * *m * sizeof(int) + *n * sizeof(int *));
    if (matrix == NULL) {
        *n = 0;
        *m = 0;
        return;
    }

    int *data = (int *)(matrix + *n);
    for (int i = 0; i < *n; i++) {
        matrix[i] = data + *m * i;
        for (int j = 0; j < *m; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                free(matrix);
                *n = 0;
                *m = 0;
                return;
            }
        }
    }
}

void output(int **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        printf("%d", matrix[i][0]);
        for (int j = 1; j < m; j++) {
            printf(" %d", matrix[i][j]);
        }
        if (i < n - 1) {
            printf("\n");
        }
    }
}

int compare_ints(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void sort_vertical(int **matrix, int n, int m, int **result_matrix) {
    int total = n * m;
    int *buffer = malloc(total * sizeof(int));
    int index = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            buffer[index++] = matrix[i][j];
        }
    }

    qsort(buffer, total, sizeof(int), compare_ints);
    index = 0;

    for (int col = 0; col < m; col++) {
        if (col % 2 == 0) {
            for (int row = 0; row < n; row++) {
                result_matrix[row][col] = buffer[index++];
            }
        } else {
            for (int row = n - 1; row >= 0; row--) {
                result_matrix[row][col] = buffer[index++];
            }
        }
    }

    free(buffer);
}

void sort_horizontal(int **matrix, int n, int m, int **result_matrix) {
    int total = n * m;
    int *buffer = malloc(total * sizeof(int));
    int index = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            buffer[index++] = matrix[i][j];
        }
    }

    qsort(buffer, total, sizeof(int), compare_ints);
    index = 0;

    for (int row = 0; row < n; row++) {
        if (row % 2 == 0) {
            for (int col = 0; col < m; col++) {
                result_matrix[row][col] = buffer[index++];
            }
        } else {
            for (int col = m - 1; col >= 0; col--) {
                result_matrix[row][col] = buffer[index++];
            }
        }
    }

    free(buffer);
}