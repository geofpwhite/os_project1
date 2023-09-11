#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    tokenizeWithStrTok(line, &token_vector);
    if (strcmp(token_vector[0], "cd") == 0) {
      chdir(token_vector[1]);
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
