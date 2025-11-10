#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arguments {
  int b, n, s, E, e, T, t, v;
} arguments;

arguments argument_parser(int argc, char **argv) {
  arguments arg = {0};
  int opt;

  struct option long_options[] = {
      {"number", no_argument, NULL, 'n'},
      {"number-nonblank", no_argument, NULL, 'b'}, 
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}
  };

  // ЦИКЛ для обработки ВСЕХ опций
  while ((opt = getopt_long(argc, argv, "bnEeTtsv", long_options, NULL)) != -1) {
    switch (opt) {
      case 'b':
        arg.b = 1;
        break;
      case 'n':
        arg.n = 1; 
        break;
      case 'e':
        arg.e = 1;
        arg.v = 1;  // -e подразумевает -v
        break;
      case 's':
        arg.s = 1;
        break;
      case 'E':
        arg.E = 1;  // -E без -v
        break;
      case 'T':
        arg.T = 1;  // -T без -v  
        break;
      case 't':
        arg.t = 1;
        arg.v = 1;  // -t подразумевает -v
        break;
      case 'v':  // Добавлена опция -v
        arg.v = 1;
        break;
      case '?':
        printf("usage: cat [-bens] [file ...]\n");
        exit(1);
    }
  }
  return arg;  // ВОЗВРАЩАЕМ результат
}

// Пример использования
int main(int argc, char **argv) {
  arguments args = argument_parser(argc, argv);
  
  printf("Флаги: b=%d, n=%d, s=%d, E=%d, e=%d, T=%d, t=%d, v=%d\n",
         args.b, args.n, args.s, args.E, args.e, args.T, args.t, args.v);
         
  return 0;
}