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

void tokenizeWithStrTok(char *input, int token_count, char ***token_vector) {
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
    (token_count)--;
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

int main() {
  while (1) {
    printf("\ncool geoff terminal \n");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    read = getline(&line, &len, stdin);
    if (feof(stdin)) {
      return 0;
    }
    if (line[0] == '\n') {
      continue;
    }
    int token_count = 1;
    char **token_vector;

    getTokenCount(line, len, &token_count);
    token_vector = (char **)malloc(sizeof(char *) * token_count + 1);
    token_vector[token_count] = NULL;
    // tokenize(line, token_count, &token_vector);
    tokenizeWithStrTok(line, token_count, &token_vector);
    if (strcmp(token_vector[0], "cd") == 0) {
      chdir(token_vector[1]);
      printf("cd detected\n");
    } else if (strcmp(token_vector[0], "pwd") == 0) {
      char dir[64];
      getcwd(dir, sizeof(dir));
      printf("%s\n", dir);
    } else if (strcmp(token_vector[0], "exit") == 0) {
      exit(0);
    } else {
      int rc = fork();
      if (rc == 0) {
        int x = execvp(token_vector[0], token_vector);
        if (x != 0) {
          perror("something went wrong chief\n");
          exit(1);
        }
        exit(0);
      } else {
        waitpid(rc, NULL, 0);
      }
    }

    free(token_vector);
  }
}
