#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void getTokenCount(char *line, int len, int *token_count) {
  for (int i = 0; i < len; ++i) {
    if (line[i] == ' ') {
      (*token_count)++;
    }
  }
}

void tokenizeWithStrTok(char *input, char ***token_vector) {
  char **hold = *token_vector;
  char *token_string;
  token_string = strtok(input, " \n\t");
  while (token_string != NULL) {
    int length = (int)strlen(token_string);
    if (token_string[length - 1] == '\n') {
      length = length - 1;
    }
    **token_vector = malloc(sizeof(char) * (length + 1));
    strncpy(**token_vector, token_string, length);
    (*token_vector)++;
    token_string = strtok(NULL, " ");
  }
  *token_vector = hold;
}

void tokenize(char *input, int token_count, char ***token_vector) {
  int index = 0;
  int count2 = token_count;
  char **hold = *token_vector;
  while (token_count > 0) {
    while (input[index] != ' ' && input[index] != '\n') {
      ++index;
    }
    **token_vector = malloc(sizeof(char) * (index + 1));
    strcpy(**token_vector, input);
    (token_count)--;
    (*token_vector)++;
    input = &input[index + 1];
    index = 0;
  }
  *token_vector = hold;
}
