//
//  main.c
//  HW4
//
//  Created by Mert Arıcan on 17.12.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER 256



/*
 * Part 1 - Counting
 *
 *
 *
 *
 *
 */

void display_matrix(int **matrix, int m, int n) {
    int i,j = 0;
    printf("\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int count_matrix(const char *x, const char *y, int m, int n) {
    int **count_matrix = malloc(sizeof(int*) * (m+1));
    int i, j = 0;

    if (count_matrix == NULL) {
        printf("malloc error!\n");
        exit(1);
    }

    for (i = 0; i < m+1; i++) {
        count_matrix[i] = malloc(sizeof(int)*(n+1));
        if (count_matrix[i] == NULL) {
            printf("malloc error!\n");
            exit(1);
        }
    }
    for (i = 0; i < m+1; i++) {
        for (j = 0; j < n+1; j++) {
            count_matrix[i][j] = 0;
        }
    }

    for (i = 1; i < m+1; i++) {
        for (j = 1; j < n+1; j++) {
            if (x[i-1] == y[j-1]) {
                count_matrix[i][j] = 1 + count_matrix[i-1][j-1];
            }
            else {
                int lhs = count_matrix[i-1][j];
                int rhs = count_matrix[i][j-1];
                count_matrix[i][j] = (lhs > rhs) ? lhs : rhs;
            }
        }
        display_matrix(count_matrix, m+1, n+1);
    }

    int result = count_matrix[m][n];
    for (i = 0; i < m+1; i++) {
        free(count_matrix[i]);
    }
    free(count_matrix);
    return result;
}



/*
 * Part 2 - Printing all lcs
 *
 *
 *
 *
 *
 *
 
 */

void addChar(char *str, char newChar) {
    int i = 0;
    while (str[i] != '\0'){i++; }
    str[i] = newChar;
}


void enrich_all(char **strList, char **dest, char elem, unsigned long size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if (strlen(strList[i]) != 0) {
            strcpy(dest[i], strList[i]);
            addChar(dest[i], elem);
        }
        else {
            if (i == 0) {
                addChar(dest[i], elem);
            }
            else {
                strcpy(dest[i], "");
            }
        }
    }
}

bool contains(char **ls, char *str, unsigned long M) {
    int i = 0;
    for (i = 0; i < M; i++) {
        if (strcmp(ls[i], str) == 0) {
            return true;
        }
    }
    return false;
}

int firstEmptyIndex(char **ls, unsigned long M) {
    int i = 0;
    for (i = 0; i < M; i++) {
        if (strlen(ls[i]) == 0) {
            return i;
        }
    }
    printf("Error!");
    exit(1);
}

void uniquelyExtend(char **l1, char **l2, unsigned long s1, unsigned long s2) {
    int i = 0;
    for (i = 0; i < s2; i++) {
        if (!contains(l1, l2[i], s1)) {
            int index = firstEmptyIndex(l1, s1);
            strcpy(l1[index], l2[i]);
        }
    }
}

void printAll(char **arr, unsigned long M);

void displayLCS(char *str1, char *str2, unsigned long m, unsigned long int n) {
    unsigned long i,j,k,l = 0;
    unsigned long _max = (m > n) ? m : n;
    unsigned long possible = _max * _max;
    char ****list = malloc(sizeof(char***) * (m+1));
    for (i = 0; i < m+1; i++) {
        list[i] = malloc(sizeof(char**) * (n+1));
        for (j = 0; j < n+1; j++) {
            list[i][j] = malloc(sizeof(char*) * possible);
            for (k = 0; k < possible; k++) {
                list[i][j][k] = malloc(sizeof(char) * (_max+1)); // +! for null char.
                for (l=0; l < _max+1; l++) {
                    list[i][j][k][l] = '\0';
                }
            }
        }
    }
    
    for (i = 1; i < m+1; i++) {
        for (j = 1; j < n+1; j++) {
            if (str1[i-1] == str2[j-1]) {
                printf("%d %d\n", (int)i, (int)j);
                enrich_all(list[i-1][j-1], list[i][j], str1[i-1], possible);
            }
            else {
                char **lhs = list[i-1][j];
                char **rhs = list[i][j-1];
                char **res = NULL;
                if (strlen(lhs[0]) == strlen(rhs[0])) {
                    uniquelyExtend(lhs, rhs, possible, possible);
                    res = lhs;
                }
                else {
                    res = (strlen(lhs[0]) > strlen(rhs[0])) ? lhs : rhs;
                }
                list[i][j] = res;
            }
        }
    }
    printAll(list[m][n], possible);
    
    
    for (i = 0; i < m+1; i++) {
        list[i] = malloc(sizeof(char**) * (n+1));
        for (j = 0; j < n+1; j++) {
            list[i][j] = malloc(sizeof(char*) * possible);
            for (k = 0; k < possible; k++) {
                free(list[i][j][k]);
            }
            free(list[i][j]);
        }
        free(list[i]);
    }
    free(list);
}


void printAll(char **arr, unsigned long M) {
    int i = 0;
    printf("\n");
    for (i = 0; i < M; i++) {
        if (strlen(arr[i]) != 0)
        printf("%s - ", arr[i]);
    }
    printf("\n");
}


int main(int argc, const char * argv[]) {
    
    char input[BUFFER];
    char input2[BUFFER];
    
    printf("string 1: ");
    fgets(input, BUFFER, stdin);
    
    printf("string 2: ");
    fgets(input2, BUFFER, stdin);
    
    int lcs = count_matrix(input, input2, (int) strlen(input)-1, (int) strlen(input2)-1);
    
    printf("Longest common substring size: %d\n", lcs);
    
    displayLCS(input, input2, strlen(input)-1, strlen(input2)-1);
    
    return 0;
}

// abcdzb - bacedab
