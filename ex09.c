#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
    int total;
} Student;

void compute_total(Student* s) {
    s->total = s->korean + s->english + s->math;
}

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("File open failed");
        return NULL;
    }
    char line[MAX_LINE_LEN];
    int capacity = 16;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);
    if (!arr) {
        perror("malloc failed");
        fclose(fp);
        return NULL;
    }
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* tmp = realloc(arr, sizeof(Student) * capacity);
            if (!tmp) {
                perror("realloc failed");
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = tmp;
        }
        Student s;
        char* token = strtok(line, ",");
        s.id = atoi(token);
        token = strtok(NULL, ",");
        strncpy(s.name, token, MAX_NAME_LEN);
        s.name[strcspn(s.name, "\n")] = '\0';
        token = strtok(NULL, ",");
        s.gender = token[0];
        token = strtok(NULL, ",");
        s.korean = atoi(token);
        token = strtok(NULL, ",");
        s.english = atoi(token);
        token = strtok(NULL, ",");
        s.math = atoi(token);
        compute_total(&s);
        arr[count++] = s;
    }
    fclose(fp);
    *out_count = count;
    return arr;
}

Student* copy_students(Student* src, int n) {
    Student* dst = malloc(sizeof(Student) * n);
    memcpy(dst, src, sizeof(Student) * n);
    return dst;
}

int has_duplicates(Student* arr, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i].id == arr[j].id)
                return 1;
        }
    }
    return 0;
}

void print_student(const Student* s) {
    printf("%d, %s, %c, %d, %d, %d, total=%d\n", s->id, s->name, s->gender, s->korean, s->english, s->math, s->total);
}

void print_result(const char* algo, const char* criteria, const char* direction, long avg_comp, long avg_mem) {
    printf("[%s] 기준: %s, %s\n", algo, criteria, direction);
    printf("  평균 비교 횟수 : %ld\n", avg_comp);
    printf("  평균 메모리 사용량 : %ld bytes\n\n", avg_mem);
}

int comp_id_asc(const Student* a, const Student* b) {
    return a->id - b->id;
}

int comp_id_desc(const Student* a, const Student* b) {
    return b->id - a->id;
}

int comp_name_asc(const Student* a, const Student* b) {
    return strcmp(a->name, b->name);
}

int comp_name_desc(const Student* a, const Student* b) {
    return -strcmp(a->name, b->name);  // 반대
}

int comp_gender_asc(const Student* a, const Student* b) {
    return (a->gender - b->gender);
}

int comp_gender_desc(const Student* a, const Student* b) {
    return (b->gender - a->gender);
}

int comp_total_asc(const Student* a, const Student* b) {
    if (a->total != b->total)
        return a->total - b->total;
    if (a->korean != b->korean)
        return a->korean - b->korean;
    if (a->english != b->english)
        return a->english - b->english;
    return a->math - b->math;
}

int comp_total_desc(const Student* a, const Student* b) {
    if (a->total != b->total)
        return b->total - a->total;
    if (a->korean != b->korean)
        return b->korean - a->korean;
    if (a->english != b->english)
        return b->english - a->english;
    return b->math - a->math;
}

const char* get_criteria_name(int criteria_id) {
    switch(criteria_id) {
        case 0: return "ID";
        case 1: return "NAME";
        case 2: return "GENDER";
        case 3: return "TOTAL";
    }
    return "?";
}

void sort_bubble(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i -1; j++) {
            (*comp_count)++;
            if (comp(&arr[j], &arr[j+1]) > 0) {
                Student tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

void sort_insertion(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = 0; 
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comp_count)++;
            if (comp(&arr[j], &key) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = key;
    }
}

void merge(Student* arr, int left, int mid, int right, int (*comp)(const Student*, const Student*), long* comp_count) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Student* L = malloc(sizeof(Student) * n1);
    Student* R = malloc(sizeof(Student) * n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        (*comp_count)++;  
        if (comp(&L[i], &R[j]) <= 0)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) 
        arr[k++] = L[i++];
    while (j < n2) 
        arr[k++] = R[j++];
    free(L);
    free(R);
}

void merge_sort_recursive(Student* arr, int left, int right, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    if (left < right) {
        int mid = left + (right-left)/2;
        merge_sort_recursive(arr, left, mid, comp, comp_count, mem_count);
        merge_sort_recursive(arr, mid+1, right, comp, comp_count, mem_count);
        merge(arr, left, mid, right, comp, comp_count);
    }
}

void sort_merge(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = sizeof(Student) * n;
    merge_sort_recursive(arr, 0, n-1, comp, comp_count, mem_count);
}

void sort_radix(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = sizeof(Student) * n;
    int max_val = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i].total > max_val) 
            max_val = arr[i].total;
    }
    for (int exp = 1; max_val/exp > 0; exp *= 10) {
        Student* output = malloc(sizeof(Student)*n);
        int count[10] = {0};
        for (int i = 0; i < n; i++) 
            count[(arr[i].total/exp)%10]++;
        for (int i = 1; i < 10; i++) 
            count[i] += count[i-1];
        for (int i = n-1; i >= 0; i--) {
            int idx = (arr[i].total/exp)%10;
            output[count[idx]-1] = arr[i];
            count[idx]--;
        }
        for (int i = 0; i < n; i++) 
            arr[i] = output[i];
        free(output);
    }
}

void sort_selection(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = 0; 
    for (int i=0;i<n-1;i++) {
        int min_idx = i;
        for (int j=i+1;j<n;j++) {
            (*comp_count)++;
            if (comp(&arr[j], &arr[min_idx]) < 0)
                min_idx = j;
        }
        if (min_idx != i) {
            Student tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

void sort_shell(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = 0;
    for (int gap = n/2; gap>0; gap/=2) {
        for (int i=gap;i<n;i++) {
            Student temp = arr[i];
            int j;
            for (j=i; j>=gap; j-=gap) {
                (*comp_count)++;
                if (comp(&arr[j-gap], &temp) > 0) 
                    arr[j] = arr[j-gap];
                else 
                break;
            }
            arr[j] = temp;
        }
    }
}

int partition(Student* arr, int low, int high, int (*comp)(const Student*, const Student*), long* comp_count) {
    int mid = low + (high - low) / 2;
    Student tmp = arr[mid];
    arr[mid] = arr[high];
    arr[high] = tmp;
    Student pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        (*comp_count)++;
        if (comp(&arr[j], &pivot) <= 0) {
            i++;
            Student t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }
    Student t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return i + 1;
}

void quick_sort_recursive(Student* arr, int low, int high, int (*comp)(const Student*, const Student*), long* comp_count) {
    if (low<high) {
        int pi = partition(arr, low, high, comp, comp_count);
        quick_sort_recursive(arr, low, pi-1, comp, comp_count);
        quick_sort_recursive(arr, pi+1, high, comp, comp_count);
    }
}

void sort_quick(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = 0;
    quick_sort_recursive(arr, 0, n-1, comp, comp_count);
}

void heapify(Student* arr, int n, int i, int (*comp)(const Student*, const Student*), long* comp_count) {
    int largest = i;
    int l = 2*i +1;
    int r = 2*i +2;
    if (l<n) { 
        (*comp_count)++; 
        if (comp(&arr[l], &arr[largest]) > 0) 
            largest=l;
    }
    if (r<n) { 
        (*comp_count)++; 
        if (comp(&arr[r], &arr[largest]) > 0) 
            largest=r;
    }
    if (largest != i) {
        Student tmp=arr[i]; 
        arr[i]=arr[largest]; 
        arr[largest]=tmp;
        heapify(arr,n,largest,comp,comp_count);
    }
}

void sort_heap(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count=0;
    *mem_count=0;
    for (int i=n/2-1;i>=0;i--) 
        heapify(arr,n,i,comp,comp_count);
    for (int i=n-1;i>=0;i--) {
        Student tmp=arr[0]; 
        arr[0]=arr[i]; 
        arr[i]=tmp;
        heapify(arr,i,0,comp,comp_count);
    }
}

typedef struct TreeNode {
    Student data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* insert_node(TreeNode* root, Student data, long* mem_count, long* comp_count) {
    if (!root) {
        TreeNode* node = malloc(sizeof(TreeNode));
        *mem_count += sizeof(TreeNode);
        node->data = data;
        node->left = node->right = NULL;
        return node;
    }
    (*comp_count)++;
    if (data.id < root->data.id)
        root->left = insert_node(root->left, data, mem_count, comp_count);
    else 
        root->right = insert_node(root->right, data, mem_count, comp_count);
    return root;
}

void inorder_traversal(TreeNode* root, Student* arr, int* idx) {
    if (!root) 
        return;
    inorder_traversal(root->left, arr, idx);
    arr[(*idx)++] = root->data;
    inorder_traversal(root->right, arr, idx);
}

void free_tree(TreeNode* root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void sort_tree(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count) {
    *comp_count = 0;
    *mem_count = 0;
    TreeNode* root = NULL;
    for (int i=0; i<n; i++) {
        root = insert_node(root, arr[i], mem_count, comp_count);
    }
    int idx = 0;
    inorder_traversal(root, arr, &idx);
    free_tree(root);
}

const char* get_algorithm_name(int algo_id) {
    switch(algo_id) {
        case 0: return "Bubble Sort";
        case 1: return "Insertion Sort";
        case 2: return "Merge Sort";
        case 3: return "Radix Sort";
        case 4: return "Selection Sort";
        case 5: return "Shell Sort";
        case 6: return "Quick Sort";
        case 7: return "Heap Sort";
        case 8: return "Tree Sort";
    }
    return "?";
}

void run_single_sort(Student* orig, int n, void (*sort_func)(Student*, int, int (*)(const Student*, const Student*), long*, long*), int (*comp)(const Student*, const Student*), const char* algo_name, const char* criteria_name, const char* direction) {
    long total_comp = 0;
    long total_mem = 0;
    int repeat = 10;
    for (int i=0;i<repeat;i++) {
        Student* arr = copy_students(orig, n);
        long comp_count=0, mem_count=0;
        sort_func(arr, n, comp, &comp_count, &mem_count);
        total_comp += comp_count;
        total_mem += mem_count;
        free(arr);
    }
    print_result(algo_name, criteria_name, direction, total_comp/repeat, total_mem/repeat);
}

int has_duplicates_by_criteria(Student* arr, int n, int criteria) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            switch(criteria) {
                case 0: 
                    if (arr[i].id == arr[j].id) 
                        return 1;
                    break;
                case 1: 
                    if (strcmp(arr[i].name, arr[j].name) == 0) 
                        return 1;
                    break;
                case 2: 
                    if (arr[i].gender == arr[j].gender) 
                        return 1;
                    break;
                case 3: 
                    if (arr[i].total == arr[j].total) 
                        return 1;
                    break;
            }
        }
    }
    return 0;
}

void run_all_sorts(Student* orig, int n) {
    int (*comparators[8])(const Student*, const Student*) = {
        comp_id_asc, comp_id_desc,
        comp_name_asc, comp_name_desc,
        comp_gender_asc, comp_gender_desc,
        comp_total_asc, comp_total_desc
    };
    const char* criteria_name[8] = {
        "ID","ID","NAME","NAME","GENDER","GENDER","TOTAL","TOTAL"
    };
    const char* direction[8] = {
        "ASC","DESC","ASC","DESC","ASC","DESC","ASC","DESC"
    };
    void (*stable_algos[4])(Student*, int, int (*)(const Student*, const Student*), long*, long*) = {
        sort_bubble, sort_insertion, sort_merge, sort_radix
    };
    void (*nonstable_algos[5])(Student*, int, int (*)(const Student*, const Student*), long*, long*) = {
        sort_selection, sort_shell, sort_quick, sort_heap, sort_tree
    };
    for (int a=0;a<4;a++) {
        for (int c=0;c<8;c++) {
            if ((c==4 || c==5) && a>=4) continue;
            run_single_sort(orig, n, stable_algos[a], comparators[c],
                            get_algorithm_name(a), criteria_name[c], direction[c]);
        }
    }
    for (int a=0;a<5;a++) {
        for (int c=0;c<8;c++) {
            if (c==4 || c==5) continue;

            if ((a==3 || a==4) && has_duplicates_by_criteria(orig, n, c/2)) continue;

            run_single_sort(orig, n, nonstable_algos[a], comparators[c],
                            get_algorithm_name(a+4), criteria_name[c], direction[c]);
        }
    }
}

int main() {
    const char* filename = "dataset_id_ascending.csv";  
    int n;
    Student* students = load_students(filename, &n);
    if (!students) {
        fprintf(stderr, "Failed to load students.\n");
        return 1;
    }
    int dup = has_duplicates(students, n);
    if (dup) {
        printf("Warning: Duplicate IDs detected. Heap and Tree sort will be skipped.\n");
    }
    printf("Loaded %d students.\n", n);
    run_all_sorts(students, n);
    free(students);
    return 0;
}
