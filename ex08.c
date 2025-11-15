#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10000
#define MAX_VALUE 1000000
#define RUNS 100

long long insertionSort(int arr[], int n) {
    long long count = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && (++count && arr[j] > key)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return count;
}

long long shellSort_basic(int arr[], int n) {
    long long count = 0;
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && (++count && arr[j - gap] > temp)) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
    return count;
}

long long shellSort_knuth(int arr[], int n) {
    long long count = 0;
    int gap = 1;
    while (gap < n / 3)
        gap = 3 * gap + 1;
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && (++count && arr[j - gap] > temp)) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
    return count;
}

void generate_random_data(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % (MAX_VALUE + 1);
}

int main() {
    int arr[SIZE], copy1[SIZE], copy2[SIZE];
    srand((unsigned)time(NULL));
    double avg_insert = 0, avg_shell_basic = 0, avg_shell_opt = 0;
    for (int run = 0; run < RUNS; run++) {
        generate_random_data(arr, SIZE);
        for (int i = 0; i < SIZE; i++) {
            copy1[i] = arr[i];
            copy2[i] = arr[i];
        }
        long long c1 = insertionSort(arr, SIZE);
        long long c2 = shellSort_basic(copy1, SIZE);
        long long c3 = shellSort_knuth(copy2, SIZE);
        avg_insert += c1;
        avg_shell_basic += c2;
        avg_shell_opt += c3;
    }
    avg_insert /= RUNS;
    avg_shell_basic /= RUNS;
    avg_shell_opt /= RUNS;
    printf("===== 평균 비교 횟수 (100회 평균) =====\n");
    printf("삽입 정렬 평균 비교 횟수     : %.0f\n", avg_insert);
    printf("쉘 정렬(기본 gap=n/2...) 평균 : %.0f\n", avg_shell_basic);
    printf("쉘 정렬(개선된 Knuth 수열) 평균 : %.0f\n", avg_shell_opt);
    return 0;
}
