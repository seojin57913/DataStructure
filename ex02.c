#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
char buf[10000];
char *str;
char arr[10000];

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

int main(void){
  scanf("%[^\n]", buf);
  trim(buf);
  str = buf;
  staArray(1);
  int maxIndex = 0;
  int nodeCount = 0;
  int leafCount = 0;

  for (int i = 1; i < 10000; i++) {
    if (isalpha(arr[i])) {
      nodeCount++;
      if (i > maxIndex)
        maxIndex = i;
      if (!isalpha(arr[i * 2]) && !isalpha(arr[i * 2 + 1])) {
        leafCount++;
      }
    }
  }
  int height = (int)log2(maxIndex);
  printf("%d,%d,%d\n", height, nodeCount, leafCount);
  return 0;
}
