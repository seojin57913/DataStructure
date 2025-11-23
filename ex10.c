#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct {
  int id;
  char name[MAX_NAME_LEN];
  char gender;
  int korean;
  int english;
  int math;
  long long product;  
} Student;

Student* load_students(const char* filename, int* out_count) {
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    perror("Failed to open file");
    return NULL;
  }
  char line[MAX_LINE_LEN];
  int capacity = 10;
  int count = 0;
  Student* arr = malloc(sizeof(Student) * capacity);
  if (!arr) {
    perror("Memory allocation failed");
    fclose(fp);
    return NULL;
  }
  fgets(line, sizeof(line), fp);
  while (fgets(line, sizeof(line), fp)) {
    if (count >= capacity) {
      capacity *= 2;
      Student* temp = realloc(arr, sizeof(Student) * capacity);
      if (!temp) {
        perror("Reallocation failed");
        free(arr);
        fclose(fp);
        return NULL;
      }
      arr = temp;
    }
    Student s;
    char* token = strtok(line, ",");
    s.id = atoi(token);
    token = strtok(NULL, ",");
    strncpy(s.name, token, MAX_NAME_LEN);
    token = strtok(NULL, ",");
    s.gender = token[0];
    token = strtok(NULL, ",");
    s.korean = atoi(token);
    token = strtok(NULL, ",");
    s.english = atoi(token);
    token = strtok(NULL, ",");
    s.math = atoi(token);
    s.product = (long long)s.korean * s.english * s.math;
    arr[count++] = s;
  }
  fclose(fp);
  Student* tight = realloc(arr, sizeof(Student) * count);
  if (!tight) {
    fprintf(stderr, "Warning: Tight reallocation failed, using original memory.\n");
    *out_count = count;
     return arr;
  }
  *out_count = count;
  return tight;
}

int linear_search(Student* arr, int n, long long target, int* comparisons) {
  *comparisons = 0;
  for (int i = 0; i < n; i++) {
    (*comparisons)++;
    if (arr[i].product == target)
      return i;
  }
  return -1;
}

long long sort_comparisons = 0;

int cmp_student(const void* a, const void* b) {
  sort_comparisons++;
  long long pa = ((Student*)a)->product;
  long long pb = ((Student*)b)->product;
  if (pa < pb) return -1;
  else if (pa > pb) return 1;
  return 0;
}

int binary_search(Student* arr, int n, long long target, int* comparisons) {
  int left = 0, right = n - 1;
  *comparisons = 0;
  while (left <= right) {
    (*comparisons)++;
    int mid = (left + right) / 2;
    if (arr[mid].product == target)
      return mid;
    else if (arr[mid].product < target)
      left = mid + 1;
    else
      right = mid - 1;
  }
  return -1;
}

int main() {
  srand(time(NULL));
  int n;
  Student* arr = load_students("dataset_id_ascending.csv", &n);
  if (!arr) 
    return 1;
  long long target = rand() % 1000001;
  printf("Random target = %lld\n", target);
  int linear_cmp = 0;
  int linear_idx = linear_search(arr, n, target, &linear_cmp);
  printf("[Linear Search] index=%d, comparisons=%d\n", linear_idx, linear_cmp);
  sort_comparisons = 0;
  qsort(arr, n, sizeof(Student), cmp_student);
  int binary_cmp = 0;
  int binary_idx = binary_search(arr, n, target, &binary_cmp);
  printf("[Binary Search] index=%d, sort_cmp=%lld, binary_cmp=%d, total=%lld\n", binary_idx, sort_comparisons, binary_cmp, sort_comparisons + binary_cmp);
  free(arr);
  return 0;
}
