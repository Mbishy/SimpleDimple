#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments *argument_parser(int argc, char *argv[]) {
  arguments arg = {0};
  int opt;
  opt = getopt_long(argc, argv, "bnsEeTtv", NULL, 0);
  switch (opt) {
    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    case 'b':
      arg.b = 1;
      break;

    default:
      break;
  }
}

int main(int argc, char *argv[]) { return 0; }
