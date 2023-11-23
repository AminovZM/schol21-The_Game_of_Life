//#include <ncurses.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGTH 25
#define SPDSTEP 5

void input_matrix_0(int **matrix, int m, int n);
void output(int **matrix, int m, int n);
int change_state(int **matrix, int m, int n);
void input_stat_matrix_0(int matrix[80][25], int m, int n);
void scanmatr(int **matrix, int m, int n);
void neigbor123(int **matrix, const int *m, const int *n, const int *i, const int *j, int *living_neighbors);
void neighbor45(int **matrix, const int *n, const int *i, const int *j, int *living_neighbors);
void neigbor678(int **matrix, const int *m, const int *n, const int *i, const int *j, int *living_neighbors);

int main() {
    int m = HEIGTH, n = WIDTH;
    int **matrix = malloc(m * n * sizeof(int) + m * sizeof(int *));
    int *ptr = (int *)(matrix + m);
    for (int i = 0; i < m; i++) matrix[i] = ptr + n * i;

    printw("123456\n");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    int speed_ms = SPDSTEP;

    input_matrix_0(matrix, m, n);
    scanmatr(matrix, m, n);
    if (freopen("/dev/tty", "r", stdin) != NULL) TRUE;
    int count = 0;
    while (1) {
        int ch = getch();
        if (count == 1) break;
        switch (ch) {
            case '=':
            case '+':
                speed_ms += 1;
                break;
            case '_':
            case '-':
                if (speed_ms > 1) speed_ms -= 1;
                break;
            case 'Q':
            case 'q':
                count++;
                endwin();
                free(matrix);
                break;
        }

        clear();
        output(matrix, m, n);
        printw("\n");
        refresh();
        usleep(200000 / speed_ms);
        if (change_state(matrix, m, n) == 0) break;
    }

    endwin();
    free(matrix);
    return 0;
}

void input_matrix_0(int **matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}

void output(int **matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (j == 0 || j == n - 1) {
                printw("|");
            } else if (i == 0 || i == m - 1) {
                printw("-");
            } else {
                if (matrix[i][j] == 0)
                    printw(" ");
                else
                    printw("#");
            }
        }
        printw("\n");
    }
}

void input_stat_matrix_0(int matrix[80][25], int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}

void neigbor123(int **matrix, const int *m, const int *n, const int *i, const int *j, int *living_neighbors) {
    if (*i == 1) {
        if (*j == 1) {
            if (matrix[*m - 2][*n - 2] == 1) (*living_neighbors)++;
        } else {
            if (matrix[*m - 2][*j - 1] == 1) (*living_neighbors)++;
        }
        if (matrix[*m - 2][*j] == 1) (*living_neighbors)++;
        if (*j == *n - 2) {
            if (matrix[*m - 2][1] == 1) (*living_neighbors)++;
        } else {
            if (matrix[*m - 2][*j + 1] == 1) (*living_neighbors)++;
        }
    } else {
        if (*j == 1) {
            if (matrix[*i - 1][*n - 2] == 1) (*living_neighbors)++;
        } else {
            if (matrix[*i - 1][*j - 1] == 1) (*living_neighbors)++;
        }
        if (matrix[*i - 1][*j] == 1) (*living_neighbors)++;
        if (*j == *n - 2) {
            if (matrix[*i - 1][1] == 1) (*living_neighbors)++;
        } else {
            if (matrix[*i - 1][*j + 1] == 1) (*living_neighbors)++;
        }
    }
}

void neighbor45(int **matrix, const int *n, const int *i, const int *j, int *living_neighbors) {
    if (*j == 1) {
        if (matrix[*i][*n - 2] == 1) (*living_neighbors)++;
    } else {
        if (matrix[*i][*j - 1] == 1) (*living_neighbors)++;
    }

    if (*j == *n - 2) {
        if (matrix[*i][1] == 1) (*living_neighbors)++;
    } else {
        if (matrix[*i][*j + 1] == 1) (*living_neighbors)++;
    }
}

void neigbor678(int **matrix, const int *m, const int *n, const int *i, const int *j, int *living_neighbors) {
    if (*i == *m - 2) {
        if (*j == 1) {
            if (matrix[1][*n - 2] == 1) (*living_neighbors)++;
        } else {
            if (matrix[1][*j - 1] == 1) (*living_neighbors)++;
        }
        if (matrix[1][*j] == 1) (*living_neighbors)++;
        if (*j == *n - 2) {
            if (matrix[1][1] == 1) (*living_neighbors)++;
        } else {
            if (matrix[1][*j + 1] == 1) (*living_neighbors)++;
        }
    } else {
        if (*j == 1) {
            if (matrix[*i + 1][*n - 2] == 1) (*living_neighbors)++;
        } else {
            if (matrix[*i + 1][*j - 1] == 1) (*living_neighbors)++;
        }
        if (matrix[*i + 1][*j] == 1) (*living_neighbors)++;
        if (*j == *n - 2) {
            if (matrix[*i + 1][1] == 1) (*living_neighbors)++;
        } else {
            if (matrix[*i + 1][*j + 1] == 1) (*living_neighbors)++;
        }
    }
}

int change_state(int **matrix, int m, int n) {
    int living_cell = 0;
    int new_matrix[m][n];
    input_stat_matrix_0(new_matrix, n, m);

    for (int i = 1; i < m - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            if (matrix[i][j] == 1) living_cell++;
            int living_neighbors = 0, dead_neighbors = 0;
            neigbor123(matrix, &m, &n, &i, &j, &living_neighbors);
            neighbor45(matrix, &n, &i, &j, &living_neighbors);
            neigbor678(matrix, &m, &n, &i, &j, &living_neighbors);
            dead_neighbors = 8 - living_neighbors;

            if (matrix[i][j] == 1) {
                if (dead_neighbors < 2)
                    new_matrix[i][j] = 0;
                else if (living_neighbors == 2 || living_neighbors == 3)
                    new_matrix[i][j] = 1;
                else if (living_neighbors > 3)
                    new_matrix[i][j] = 0;
            } else {
                if (living_neighbors == 3) new_matrix[i][j] = 1;
            }
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) matrix[i][j] = new_matrix[i][j];
    }

    return living_cell;
}

void scanmatr(int **matrix, int m, int n) {
    for (m = 0; m < HEIGTH; m++) {
        for (n = 0; n < WIDTH; n++) {
            scanf("%d", &matrix[m][n]);
        }
    }
}