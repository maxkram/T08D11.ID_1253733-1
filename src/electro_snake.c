#include <stdio.h>
#include <stdlib.h>

void input_size(int *rows, int *cols, int *flag);
int **input_matrix(int rows, int cols, int *flag);
void output(int **matrix, int rows, int cols, int flag);
void copy_matrix(int **src, int **dst, int rows, int cols);
void sort_and_fill(int **matrix, int rows, int cols, int vertical);
void fill_snake_vertical(int **matrix, int rows, int cols, const int *buf);
void fill_snake_horizontal(int **matrix, int rows, int cols, const int *buf);
int cmp_int(const void *a, const void *b);

int main() {
    int **matrix = NULL, **matrix_v = NULL;
    int rows, cols, flag = 1;
    int result = 0;

    input_size(&rows, &cols, &flag);

    if (flag) {
        matrix = input_matrix(rows, cols, &flag);
    }

    if (flag) {
        // Create a copy for vertical snake sorting
        matrix_v = malloc(rows * sizeof(int *));
        if (matrix_v != NULL) {
            matrix_v[0] = malloc(rows * cols * sizeof(int));
            for (int i = 1; i < rows; i++) {
                matrix_v[i] = matrix_v[0] + cols * i;
            }
            copy_matrix(matrix, matrix_v, rows, cols);

            // Sort and output vertical snake
            sort_and_fill(matrix_v, rows, cols, 1);
            output(matrix_v, rows, cols, flag);
            printf("\n");
            printf("\n");
            // Sort and output horizontal snake
            sort_and_fill(matrix, rows, cols, 0);
            output(matrix, rows, cols, flag);

            free(matrix_v[0]);
            free(matrix_v);
        } else {
            flag = 0;
        }
    }

    if (!flag) {
        printf("n/a");
        result = 1;
    }

    if (matrix != NULL) {
        free(matrix[0]);
        free(matrix);
    }

    return result;
}

void input_size(int *rows, int *cols, int *flag) {
    if (scanf("%d %d", rows, cols) != 2 || *rows <= 0 || *cols <= 0) {
        *flag = 0;
    }
}

int **input_matrix(int rows, int cols, int *flag) {
    int **matrix = NULL;

    matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) {
        *flag = 0;
        return NULL;
    }

    matrix[0] = malloc(rows * cols * sizeof(int));
    if (matrix[0] == NULL) {
        free(matrix);
        *flag = 0;
        return NULL;
    }

    for (int i = 1; i < rows; i++) {
        matrix[i] = matrix[0] + cols * i;
    }

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (scanf("%d", &matrix[r][c]) != 1) {
                *flag = 0;
                break;
            }
        }
        if (!*flag) {
            break;
        }
    }

    if (!*flag) {
        free(matrix[0]);
        free(matrix);
        matrix = NULL;
    }

    return matrix;
}

void output(int **matrix, int rows, int cols, int flag) {
    if (!flag) {
        return;
    }

    for (int i = 0; i < rows; i++) {
        printf("%d", matrix[i][0]);
        for (int j = 1; j < cols; j++) {
            printf(" %d", matrix[i][j]);
        }
        if (i < rows - 1) {
            printf("\n");
        }
    }
}

void copy_matrix(int **src, int **dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

void sort_and_fill(int **matrix, int rows, int cols, int vertical) {
    int total = rows * cols;
    int *buf = malloc(total * sizeof(int));
    int idx = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            buf[idx++] = matrix[i][j];
        }
    }

    qsort(buf, total, sizeof(int), cmp_int);

    if (vertical) {
        fill_snake_vertical(matrix, rows, cols, buf);
    } else {
        fill_snake_horizontal(matrix, rows, cols, buf);
    }

    free(buf);
}

void fill_snake_vertical(int **matrix, int rows, int cols, const int *buf) {
    int idx = 0;

    for (int col = 0; col < cols; col++) {
        if (col % 2 == 0) {
            for (int row = 0; row < rows; row++) {
                matrix[row][col] = buf[idx++];
            }
        } else {
            for (int row = rows - 1; row >= 0; row--) {
                matrix[row][col] = buf[idx++];
            }
        }
    }
}

void fill_snake_horizontal(int **matrix, int rows, int cols, const int *buf) {
    int idx = 0;

    for (int row = 0; row < rows; row++) {
        if (row % 2 == 0) {
            for (int col = 0; col < cols; col++) {
                matrix[row][col] = buf[idx++];
            }
        } else {
            for (int col = cols - 1; col >= 0; col--) {
                matrix[row][col] = buf[idx++];
            }
        }
    }
}

int cmp_int(const void *a, const void *b) { return (*(const int *)a - *(const int *)b); }