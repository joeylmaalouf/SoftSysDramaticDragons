/* parsing.c
 * Contains functions for parsing and
 * checking input characters and strings. */

 #include "shell.h"

/* parse: checks an individual character and either
 *        add it to a currently-building command or
 *        start a new command based on the presence
 *        of whitespace or other separators
 * c: character to parse
 * args: array of character arrays to build commands into
 * state: state information about the current command
 * returns: flag telling main code whether a command is fully parsed */
bool parse (char c, char args[BUFSIZE][BUFSIZE], State* state) {
  if (state->in_comment && (c != '\n')) {
    return false;
  }
  switch (c) {
    case '"':
      state->in_quote = !state->in_quote;
      return false;
    case EOF:
    case '\n':
      state->in_comment = false;
      state->in_whitespace = false;
    case '\0':
    case ';':
      /* characters separating commands */
      if (!state->in_quote) {
        if (!state->in_whitespace) {
          args[state->i_cmd][state->i_char] = '\0';
          num_args = state->i_cmd + 1;
          state->i_cmd = 0;
          state->i_char = 0;
        }
        state->in_whitespace = true;
        return true;
      }
    case ' ':
    case '\t':
      /* characters separating arguments */
      if (!state->in_quote) {
        if (!state->in_whitespace) {
          args[state->i_cmd][state->i_char] = '\0';
          ++(state->i_cmd);
          state->i_char = 0;
        }
        state->in_whitespace = true;
        return false;
      }
    default:
      /* cases dependent on other flags */
      if ((c == '#') && !state->in_quote) {
        state->in_comment = true;
        return false;
      }
      /* characters composing tokens */
      args[state->i_cmd][state->i_char] = c;
      ++(state->i_char);
      state->in_whitespace = false;
      return false;
  }
}

/* is_blank: checks if the given string is just whitespace
 * string: the string to parse
 * returns: flag saying whether the string is only whitespace */
bool is_blank (char* string) {
  int l = strlen(string);
  size_t i;
  if (l == 0) {
    return true;
  }
  for (i = 0; i < l; ++i) {
    if (!isspace(string[i])) {
      return false;
    }
  }
  return true;
}
