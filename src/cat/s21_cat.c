#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments* argument_parser(int argc, char* argv[]) {
  arguments arg = {0};
  struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                  {"number", no_argument, NULL, 'n'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};
  int opt;
  opt = getopt_long(argc, argv, "bnsEeTt", long_options, 0);
  switch (opt) {
    case 'b':
      arg.b = 1;
      break;

    case 'n':
      arg.n = 1;
      break;

    case 's':
      arg.s = 1;
      break;

    case 'e':
      arg.E = 1;
      arg.v = 1;
      break;

    case 'E':
      arg.E = 1;
      break;

    case 't':
      arg.T = 1;
      arg.v = 1;
      break;

    case 'T':
      arg.T = 1;
      break;

    case '?':
      perror("ERROR");
      exit(1);
      break;

    default:
      break;
  }
}

void outline(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
}

void readfile() {}

int main(int argc, char* argv[]) {
  FILE* f = fopen(argv[1], "r");
  char* line = NULL;
  size_t memline = 0;
  int read = 0;
  read = getline(&line, &memline, f);
  while (read != -1) {
    outline(line, read);
    read = getline(&line, &memline, f);
  }
  fclose(f);
  return 0;
}
