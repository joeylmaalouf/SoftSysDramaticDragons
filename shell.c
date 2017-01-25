#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* split:
   this function takes in a line and a destination array,
   and populates the array with tokens split from the line,
   split by whitespace unless inside a quote block */
void split (char* line, char tokens[512][256]) {
  char current[256];
  int char_index = 0;
  int token_index = 0;
  int in_quote = 0;
  int in_whitespace = 1;
  int i;
  for (i = 0; i < strlen(line); ++i) {
    switch(line[i]) {
      case '"':
        /* if we encounter an argument in quotes, we won't split
           until we reach a split character after the end quote */
        in_quote = !in_quote;
        break;
      case ' ':
      case '\t':
      case '\n':
      case '\0':
      case EOF:
        /* if we've hit a splitting character, but we're
           inside a quote block, keep adding to the token */
        if (in_quote) {
          current[char_index++] = line[i];
        }
        /* but if we've hit a splitting character outside a
           quote block, and we're not in the same whitespace,
           put our current token into the list and start fresh */
        else if (!in_whitespace) {
          snprintf(tokens[token_index++], 256, "%s", current);
          memset(current, '\0', sizeof(current));
          char_index = 0;
        }
        in_whitespace = 1;
        break;
      default:
        /* if we haven't yet hit a token to split on,
           keep adding the characters to the current token */
        in_whitespace = 0;
        if (!in_whitespace) {
          current[char_index++] = line[i];
        }
    }
  }
  /* catch any remaining tokens, in case the
     file doesn't end in a space/newline/etc. */
  if (current[0] != '\0') {
    snprintf(tokens[token_index], 256, "%s", current);
  }
}

int main (int argc, char* argv[]) {
  FILE* fp;
  size_t len;
  ssize_t read;
  char* line;
  char tokens[512][256];
  /* one command can hold 512 tokens with 256 characters each */

  fp = fopen(argv[1], "r");
  while ((read = getline(&line, &len, fp)) != -1) {
    split(line, tokens);
    // printf("command: %s\n args: %s %s\n\n", tokens[0], tokens[1], tokens[2]);
    memset(tokens, '\0', sizeof(tokens));
  }

  return 0;
}
