#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INSERTION_CUTOFF 16
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

typedef struct TreeNode {
    Student data;
    struct TreeNode* left;
    struct TreeNode* right;
    int height;
} TreeNode;

void compute_total(Student* s) {
    s->total = s->korean + s->english + s->math;
}

Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;
    char line[MAX_LINE_LEN];
    int capacity = 16;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);
    if (!arr) { fclose(fp); return NULL; }
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* tmp = realloc(arr, sizeof(Student) * capacity);
            if (!tmp) { 
                free(arr); 
                fclose(fp); 
                return NULL; 
            }
            arr = tmp;
        }
        Student s;
        line[strcspn(line, "\r\n")] = '\0';
        char* token = strtok(line, ","); s.id = atoi(token);
        token = strtok(NULL, ","); strncpy(s.name, token, MAX_NAME_LEN - 1);
        token = strtok(NULL, ","); s.gender = token[0];
        token = strtok(NULL, ","); s.korean = atoi(token);
        token = strtok(NULL, ","); s.english = atoi(token);
        token = strtok(NULL, ","); s.math = atoi(token);
        compute_total(&s);
        arr[count++] = s;
    }
    fclose(fp);
    *out_count = count;
    return arr;
}

Student* copy_students(Student* src, int n) {
    Student* dst = malloc(sizeof(Student) * n);
    if (dst) 
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

void print_result(const char* algo, const char* criteria, const char* direction, long avg_comp, long avg_mem) {
    printf("[%s] 기준: %s %s\n", algo, criteria, direction);
    printf("  평균 비교 횟수 : %ld\n", avg_comp);
    printf("  평균 메모리 사용량 : %ld bytes\n\n", avg_mem);
}

int comp_id_asc(const Student* a, const Student* b){ 
    return a->id - b->id; 
}

int comp_id_desc(const Student* a, const Student* b){ 
    return b->id - a->id; 
}

int comp_name_asc(const Student* a, const Student* b){ 
    return strcmp(a->name,b->name); 
}

int comp_name_desc(const Student* a, const Student* b){ 
    return -strcmp(a->name,b->name); 
}

int comp_gender_asc(const Student* a, const Student* b){ 
    return a->gender - b->gender; 
}

int comp_gender_desc(const Student* a, const Student* b){ 
    return b->gender - a->gender; 
}

int comp_total_asc(const Student* a, const Student* b){
    if (a->total != b->total) 
        return a->total - b->total;
    if (a->korean != b->korean) 
        return a->korean - b->korean;
    if (a->english != b->english) 
        return a->english - b->english;
    return a->math - b->math;
}

int comp_total_desc(const Student* a, const Student* b){
    if (a->total != b->total) 
        return b->total - a->total;
    if (a->korean != b->korean) 
        return b->korean - a->korean;
    if (a->english != b->english) 
        return b->english - a->english;
    return b->math - a->math;
}

void sort_shell_improved(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count){
    *comp_count = 0;
    *mem_count = 0;
    int gaps[] = {1,5,19,41,109,209,505,929,2161,3905,8929,16001,36289};
    int gsize = sizeof(gaps)/sizeof(int);
    int gi = gsize-1;
    while(gi >= 0 && gaps[gi] > n) 
        gi--;
    for(; gi >= 0; gi--){
        int gap = gaps[gi];
        for(int i=gap; i<n; i++){
            Student tmp = arr[i];
            int j=i;
            for(; j>=gap; j-=gap){
                (*comp_count)++;
                if(comp(&arr[j-gap], &tmp) > 0)
                    arr[j] = arr[j-gap];
                else 
                break;
            }
            arr[j] = tmp;
        }
    }
}

void insertion_range(Student* arr, int l, int r, int (*comp)(const Student*, const Student*), long* comp_count) {
    for(int i=l+1;i<=r;i++){
        Student key = arr[i];
        int j = i-1;
        while(j>=l){
            (*comp_count)++;
            if(comp(&arr[j], &key) > 0){
                arr[j+1] = arr[j];
                j--;
            } 
            else 
                break;
        }
        arr[j+1] = key;
    }
}

int median3(Student* arr, int a, int b, int c, int (*comp)(const Student*, const Student*), long* comp_count){
    (*comp_count)++; 
    if(comp(&arr[a], &arr[b]) > 0){
        Student t=arr[a]; 
        arr[a]=arr[b]; 
        arr[b]=t;
    }
    (*comp_count)++; 
    if(comp(&arr[a], &arr[c]) > 0){
        Student t=arr[a]; 
        arr[a]=arr[c]; 
        arr[c]=t;
    }
    (*comp_count)++; 
    if(comp(&arr[b], &arr[c]) > 0){
        Student t=arr[b]; 
        arr[b]=arr[c]; 
        arr[c]=t;
    }
    return b;
}

int hoare_partition(Student* arr, int l, int r, int (*comp)(const Student*, const Student*), long* comp_count) {
    int m = l + (r-l)/2;
    int piv = median3(arr, l, m, r, comp, comp_count);
    Student pivot = arr[piv];
    int i = l-1;
    int j = r+1;
    while(1){
        do{ 
            i++; 
            (*comp_count)++; 
        } while(comp(&arr[i], &pivot) < 0);
        do{ 
            j--; 
            (*comp_count)++; 
        } while(comp(&arr[j], &pivot) > 0);
        if(i >= j) 
            return j;
        Student t = arr[i]; 
        arr[i]=arr[j]; 
        arr[j]=t;
    }
}

void qsort_recursive(Student* arr, int l, int r, int (*comp)(const Student*, const Student*), long* comp_count) {
    if(r-l < INSERTION_CUTOFF){
        insertion_range(arr, l, r, comp, comp_count);
        return;
    }
    int p = hoare_partition(arr, l, r, comp, comp_count);
    qsort_recursive(arr, l, p, comp, comp_count);
    qsort_recursive(arr, p+1, r, comp, comp_count);
}

void sort_quick_improved(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count){
    *comp_count = 0;
    *mem_count = 0;
    qsort_recursive(arr, 0, n-1, comp, comp_count);
}

int height(TreeNode* N){ 
    return N? N->height:0; 
}

int max2(int a,int b){ 
    return a>b?a:b; 
}

int get_balance(TreeNode* N){ 
    return N? height(N->left)-height(N->right):0; 
}

TreeNode* right_rotate(TreeNode* y){
    TreeNode* x=y->left;
    TreeNode* T2=x->right;
    x->right=y;
    y->left=T2;
    y->height = max2(height(y->left), height(y->right))+1;
    x->height = max2(height(x->left), height(x->right))+1;
    return x;
}

TreeNode* left_rotate(TreeNode* x){
    TreeNode* y=x->right;
    TreeNode* T2=y->left;
    y->left=x;
    x->right=T2;
    x->height=max2(height(x->left),height(x->right))+1;
    y->height=max2(height(y->left),height(y->right))+1;
    return y;
}

TreeNode* insert_node_avl(TreeNode* node, Student d, long* mem_count, long* comp_count){
    if(!node){
        TreeNode* nw = malloc(sizeof(TreeNode));
        *mem_count += sizeof(TreeNode);
        nw->data=d; 
        nw->left=nw->right=NULL; 
        nw->height=1;
        return nw;
    }
    (*comp_count)++;
    if(d.id < node->data.id)
        node->left = insert_node_avl(node->left, d, mem_count, comp_count);
    else if(d.id > node->data.id)
        node->right = insert_node_avl(node->right, d, mem_count, comp_count);
    else 
        return node;
    node->height = 1 + max2(height(node->left), height(node->right));
    int bal = get_balance(node);
    if(bal > 1 && d.id < node->left->data.id)
        return right_rotate(node);
    if(bal < -1 && d.id > node->right->data.id)
        return left_rotate(node);
    if(bal > 1 && d.id > node->left->data.id){
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if(bal < -1 && d.id < node->right->data.id){
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    return node;
}

void inorder(TreeNode* root, Student* arr, int* idx){
    if(!root) return;
    inorder(root->left, arr, idx);
    arr[(*idx)++] = root->data;
    inorder(root->right, arr, idx);
}

void free_tree(TreeNode* r){
    if(!r) 
        return;
    free_tree(r->left);
    free_tree(r->right);
    free(r);
}

void sort_tree_avl(Student* arr, int n, int (*comp)(const Student*, const Student*), long* comp_count, long* mem_count){
    *comp_count = *mem_count = 0;
    TreeNode* root=NULL;
    for(int i=0;i<n;i++)
        root = insert_node_avl(root, arr[i], mem_count, comp_count);
    int idx=0;
    inorder(root, arr, &idx);
    free_tree(root);
}

const char* get_algorithm_name(int id){
    switch(id){
        case 0: 
            return "Shell Sort (Sedgewick)";
        case 1: 
            return "Quick Sort (Hoare+Median)";
        case 2: 
            return "Tree Sort (AVL)";
    }
    return "?";
}

void run_single_sort(Student* orig, int n, void (*sortf)(Student*,int,int (*)(const Student*,const Student*),long*,long*), const char* algo, const char* crit, const char* dir, int (*comp)(const Student*,const Student*)){
    long sumc=0, summ=0;
    int repeat=10;
    for(int i=0;i<repeat;i++){
        Student* arr = copy_students(orig,n);
        long cc=0, mm=0;
        sortf(arr, n, comp, &cc, &mm);
        sumc+=cc; summ+=mm;
        free(arr);
    }
    print_result(algo, crit, dir, sumc/repeat, summ/repeat);
}

void run_selected_sorts(Student* orig, int n){
    int (*comparators[8])(const Student*,const Student*) = {
        comp_id_asc, comp_id_desc,
        comp_name_asc, comp_name_desc,
        comp_gender_asc, comp_gender_desc,
        comp_total_asc, comp_total_desc
    };
    const char* cname[8]={"ID","ID","NAME","NAME","GENDER","GENDER","TOTAL","TOTAL"};
    const char* dname[8]={"ASC","DESC","ASC","DESC","ASC","DESC","ASC","DESC"};
    void (*algos[3])(Student*,int,int (*)(const Student*,const Student*),long*,long*) = {
        sort_shell_improved, sort_quick_improved, sort_tree_avl
    };
    for(int a=0;a<3;a++){
        for(int c=0;c<8;c++){
            if((a==0||a==1) && (c==4||c==5)) 
                continue;
            if(a==2){
                if(c!=0 && c!=1) 
                    continue;
                if(has_duplicates(orig,n)) 
                    continue;
            }
            run_single_sort(
                orig, n,
                algos[a],
                get_algorithm_name(a),
                cname[c], dname[c],
                comparators[c]
            );
        }
    }
}

int main(){
    const char* fname = "dataset_id_ascending.csv";
    int n;
    Student* students = load_students(fname, &n);
    if(!students){
        printf("Load error\n");
        return 1;
    }
    printf("Loaded %d students.\n", n);
    if(has_duplicates(students,n))
        printf("Warning: Duplicate IDs exist → AVL Tree Sort skipped\n");
    run_selected_sorts(students, n);
    free(students);
    return 0;
}
