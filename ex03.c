#include <stdio.h>
#include <ctype.h>
#include <string.h>
char buf[10000];
char arr[10000];
int stack[10000];
int stack2[10000];
char *str;

char *trim(char *buf) {
  char *dst = buf;
  while (*buf) {
    if (!isspace(*buf))
      *dst++ = *buf;
    buf++;
  }
  *dst = '\0';
  return dst;
}

char readNode() {
    if (*str == '(' || *str == ')')
        return 0;  
    return *str++;
}

void staArray(int index) {
  if (*str == '\0')
    return;
  else if (*str == '(') {
    str++;
    char node = readNode();
    if (node == 0)
      return;
    arr[index] = node;
    if(*str == '('){
      staArray(index * 2);
      staArray(index * 2 + 1);
    }
  }
  else if(isalpha(*str)){
    char node = readNode();
    if (node != 0) {
      arr[index] = node;
      if(*str == '('){
        staArray(index * 2);
        staArray(index * 2 + 1);
      }
      if (*str == ')')
        str++;
    }
  }
  else if(*str == ')')
    str++;
}

void preorder(int root) {
  printf("pre-order: ");
  int top = -1;
  if (arr[root] == '\0') return;
  stack[++top] = root;
  while (top != -1) {
    int idx = stack[top--];
    printf("%c ", arr[idx]);
    int right = idx * 2 + 1;
    int left = idx * 2;
    if (arr[right] != '\0') stack[++top] = right;
    if (arr[left] != '\0') stack[++top] = left;
  }
}

void inorder(int root) {
  printf("\nin-order: ");
  int top = -1;
  int current = root;
  while (top != -1 || (current < 10000 && arr[current] != '\0')) {
    while (current < 10000 && arr[current] != '\0') {
      stack[++top] = current;
      current = current * 2;
    }
    current = stack[top--];
    printf("%c ", arr[current]);
    current = current * 2 + 1;
  }
}

void postorder(int root) {
  printf("\npost-order: ");
  if (arr[root] == '\0')
    return;
  int top1 = -1, top2 = -1;
  stack[++top1] = root;
  while (top1 != -1) {
    int idx = stack[top1--];
    stack2[++top2] = idx;
    int left = idx * 2;
    int right = idx * 2 + 1;
    if (arr[left] != '\0') stack[++top1] = left;
    if (arr[right] != '\0') stack[++top1] = right;
  }
  while (top2 != -1) {
    printf("%c ", arr[stack2[top2--]]);
  }
}

int main(void){
  scanf("%[^\n]", buf);
  trim(buf);
  str = buf;
  staArray(1);
  preorder(1);
  inorder(1);
  postorder(1);
  return 0;
}
