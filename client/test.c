# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>

#include<stdio.h>
#include <string.h>

int main() {
  FILE *source = fopen("../client/anh.jpeg", "rb");
  FILE *target = fopen("test.jpeg", "wb");
  FILE *tmp = fopen("test.tmp", "wb");
  if(!source){
    printf("error");
    exit(1);
  }
  char ch;
  char buf[100];
  int i = 0;
  int count = 0;
  while(!feof(source)){
    int x = fread(buf, sizeof(buf), 1, source);
    int y = fwrite(buf, sizeof(buf), 1, target);
    count += y;
  }
  printf("%d\n", count);
  return 0;
}