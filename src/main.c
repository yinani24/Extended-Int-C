#include "APInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LEN 256

int main(int argc, char const *argv[]) {

  // Open the input and output files, use stdin and stdout if not configured.
  FILE *output = NULL;
  int outputGiven = (argc >= 3);
  if (!outputGiven)
    output = stdout;
  else
    output = fopen(argv[2], "w");

  FILE *input = NULL;
  int inputGiven = (argc >= 2);
  if (!inputGiven)
    input = stdin;
  else
    input = fopen(argv[1], "r");

  char * number = NULL;
  char * line = NULL;
  size_t len = 0;

  freopen(argv[2], "w", stdout);

  // if(dup2(output, STDOUT_FILENO) == -1){
  //   perror("Bad Dup2");
  // }

  if(getline(&number, &len, input) != -1){
    number[strcspn(number, "\r\n")] = '\0';
  }

  int num = atoi(number);
  
  free(number);

  APInt sub[num];

  for(int i = 0; i < num; i++){
    sub[i].size = 0;
  }
  
  int k = 0;
  while(getline(&line, &len, input) != -1){
    
    line[strcspn(line, "\r\n")] = '\0';
    
    if(strcmp(line, "END") == 0){
      for(int i = 0; i < num; i++){
        destroy_APInt(&sub[i]);
      }
      break;
    }
    else if(strcmp(line, "DUMP") == 0){
      dump(sub, num);
      printf("\n");
    }
    else if(strcmp(line, "UINT64") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        uint64_t in64 = strtoull(line,NULL,10);
        conversion_from_uint64(&sub[k], in64);
        k++;
      } 
    }
    else if(strcmp(line, "HEX_STRING") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        conversion_hexstring_APInt(&sub[k], line);
        k++;
      }
    }
    else if(strcmp(line, "CLONE") == 0){  
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        int f = atoi(line);
        clone(&sub[k], &sub[f]);
        k++;
      }
    }
    else if(strcmp(line, "ADD") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        int temp_arr[3];
        int temp = 0;
        char *token = strtok(line, " ");
        while(token != NULL) {
            temp_arr[temp] = atoi(token);
            token = strtok(NULL, " ");
            temp++;
        }
        ADD(&sub[temp_arr[0]], &sub[temp_arr[1]], &sub[temp_arr[2]]);
      } 
    }
    else if(strcmp(line, "SHL") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        uint64_t temp_arr[3];
        int temp = 0;
        char *token = strtok(line, " ");
        while(token != NULL) {
            temp_arr[temp] = strtoull(token, NULL, 10);
            token = strtok(NULL, " ");
            temp++;
        }
        SHL(&sub[temp_arr[0]], &sub[temp_arr[1]], temp_arr[2]);
      } 
    }
    else if(strcmp(line, "MUL_UINT64") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        uint64_t temp_arr[3];
        int temp = 0;
        char *token = strtok(line, " ");
        while(token != NULL) {
            temp_arr[temp] = strtoull(token, NULL, 10);
            token = strtok(NULL, " ");
            temp++;
        }
        mul_uint64(&sub[temp_arr[0]], &sub[temp_arr[1]], temp_arr[2]);
      } 
    }
    else if(strcmp(line, "MUL_APINT") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        int temp_arr[3];
        int temp = 0;
        char *token = strtok(line, " ");
        while(token != NULL) {
            temp_arr[temp] = atoi(token);
            token = strtok(NULL, " ");
            temp++;
        }
        mul_APInt(&sub[temp_arr[0]], &sub[temp_arr[1]], &sub[temp_arr[2]]);
      } 
    }
    else if(strcmp(line, "CMP") == 0){
      if(getline(&line, &len, input) != -1){
        line[strcspn(line, "\r\n")] = '\0';
        uint64_t temp_arr[2];
        int temp = 0;
        char *token = strtok(line, " ");
        while(token != NULL) {
            temp_arr[temp] = strtoull(token, NULL, 10);
            token = strtok(NULL, " ");
            temp++;
        }
        cmp(&sub[temp_arr[0]], &sub[temp_arr[1]]);
      } 
    }
    else{
      for(int i = 0; i < num; i++){
        destroy_APInt(&sub[i]);
      }
      break;
      // free(line);
      //return 0;
    }
  }
 
  free(line);
  // Close the files we opened.
  if (outputGiven)
    fclose(output);
  if (inputGiven)
    fclose(input);
  return 0;
}

// change fgets to getline
// use memmove for left shifting only does it by bytes
// bytes can pass all the test cases as

//r ../input/0.txt ../test.txt