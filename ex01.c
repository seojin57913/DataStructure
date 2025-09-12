#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
char buf[10000];
int i;

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
  char *p = buf;
  int result = check_binary_tree(&p);
  if(result)
    printf("TRUE\n");
  else
    printf("FALSE\n");
  return 0;
}

