#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int dataArray[100];

typedef struct Node {
  int data;
  struct Node* left;
  struct Node* right;
} Node;

Node* createNode(int value) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  if (!newNode) {
    printf("메모리 할당 실패\n");
    exit(1);
  }
  newNode->data = value;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

int treeInsert(Node** root, int value) {
  if (*root == NULL) {
    *root = createNode(value);
    return 1;
  }
  if (value == (*root)->data)
    return 0;
  else if (value < (*root)->data)
    return treeInsert(&((*root)->left), value);
  else
    return treeInsert(&((*root)->right), value);
}

void freeTree(Node* root) {
  if (root == NULL) return;
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}

int arrSearch(int arr[], int size, int target) {
  int searchCount = 0;
  for (int i = 0; i < size; i++) {
    searchCount++;
    if (arr[i] == target)
      return searchCount;
  }
  return searchCount;
}

int treeSearch(Node* root, int target) {
  int searchCount = 0;
  Node* current = root;
  while (current != NULL) {
    searchCount++;
    if (target == current->data)
      return searchCount;
    else if (target < current->data)
       current = current->left;
    else
      current = current->right;
  }
  return searchCount;
}

int main() {
  srand((unsigned int)time(NULL));
  Node* root = NULL;
  int inserted = 0;
  int randomNumber;
  printf("랜덤 생성 숫자들: ");
  while (inserted < 100) {
    randomNumber = rand() % 1001;
    if (treeInsert(&root, randomNumber)) {
      dataArray[inserted] = randomNumber;
      printf("%d, ", randomNumber);
      inserted++;
    }
  }
  int targetNumber;
  printf("\n\n탐색할 숫자: ");
  scanf("%d", &targetNumber);
  int arrCount = arrSearch(dataArray, 100, targetNumber);
  int treeCount = treeSearch(root, targetNumber);
  printf("\n배열 선형 탐색: %d회 탐색\n", arrCount);
  printf("이진 탐색 트리: %d회 탐색\n", treeCount);
  freeTree(root);
  return 0;
}
