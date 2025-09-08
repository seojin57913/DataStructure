#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define ERROR -1
#define TRUE 1
#define FALSE 0
char buf[10000];
int i;

int check_paren(char *buf, int i){
  int leftParen = 0;
  int rightParen = 0;
  if(buf[i] == '('){
    i++;
    leftParen++;
    while(buf[i]){
      if(buf[i] == '('){
        i++;
        leftParen++;
      }
      else if(buf[i] == ')'){
        if(leftParen < rightParen)
          return ERROR;
        i++;
        rightParen++;
      }
      else
        i++;
    }
    if(leftParen == rightParen)
      return TRUE;
  }
  return ERROR;
}

int check_root(char *buf, int i){
  int leftParen = 0;
  int rightParen = 0;
  int rootCount = 0;
  while(buf[i+1]){
    if(isalpha(buf[i+1])){
      i++;
      rootCount++;
    }
    else if(buf[i+1] == '('){
      i++;
      leftParen++;
      while(leftParen != rightParen){
        if(buf[i+1] == '(')
          leftParen++;
        else if(buf[i+1] == ')')
          rightParen++;
        i++;
      }
    }
    else
      i++;
  }
  if(rootCount == 1)
    return TRUE;
  return ERROR;
}

int check_binary_tree(char **pbuf){
  int nodeCount = 0;
  while(**pbuf){
    if(isspace(**pbuf))
      (*pbuf)++;
    else if(**pbuf == '('){
      (*pbuf)++;
      if(check_binary_tree(pbuf) == FALSE)
        return FALSE;
      (*pbuf)++;
    }
    else if(**pbuf == ')'){
      if(nodeCount <= 2)
        return TRUE;
      return FALSE;
    }
    else if(isalpha(**pbuf)){
      nodeCount++;
      (*pbuf)++;
    }
  }
  return TRUE;
}

int main(void){
  fgets(buf, sizeof(buf), stdin);
  buf[strcspn(buf, "\n")] = '\0';
  while(isspace(buf[i]))
    i++;
  if(check_paren(buf, i) == ERROR || check_root(buf, i) == ERROR){
    printf("ERROR\n");
    return 0;
  }
  char *p = buf;
  int result = check_binary_tree(&p);
  if(result)
    printf("TRUE\n");
  else
    printf("FALSE\n");
  return 0;
}