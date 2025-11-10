#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct flg {
  int b;  // number-nonblank
  int e;  // show ends (with -v)
  int n;  // number
  int s;  // squeeze-blank
  int t;  // show tabs (with -v)
  int v;  // show non-printing
} flg_t;

int flg_parser(int argc, char *argv[], flg_t *flg);
int read_file(const char *filename, flg_t *flg);
void process_char(int c, flg_t *flg, int *line_num, int *empty_count, int *last_char);
void print_usage(const char *program_name);

int main(int argc, char *argv[]) {
  int err = 0;
  flg_t flg = {0};
  
  err = flg_parser(argc, argv, &flg);
  if (err != 0) {
    return err;
  }

  // Если файлы не указаны, читаем из stdin
  if (optind >= argc) {
    err = read_file(NULL, &flg);
  } else {
    // Обрабатываем все указанные файлы
    for (int i = optind; i < argc; i++) {
      int file_err = read_file(argv[i], &flg);
      if (file_err != 0) {
        err = file_err;  // Запоминаем первую ошибку
      }
    }
  }
  
  return err;
}

int flg_parser(int argc, char *argv[], flg_t *flg) {
  int c;
  int ret = 0;
  int index = 0;
  static struct option options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {"show-ends", no_argument, 0, 'E'},
      {"show-tabs", no_argument, 0, 'T'},
      {"show-nonprinting", no_argument, 0, 'v'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0},
  };

  while ((c = getopt_long(argc, argv, "bEnstvTeh", options, &index)) != -1) {
    switch (c) {
      case 'b':
        flg->b = 1;
        break;

      case 'E':
        flg->e = 1;
        break;

      case 'n':
        flg->n = 1;
        break;

      case 's':
        flg->s = 1;
        break;

      case 'T':
        flg->t = 1;
        break;

      case 'v':
        flg->v = 1;
        break;

      case 'e':  // equivalent to -vE
        flg->e = 1;
        flg->v = 1;
        break;

      case 't':  // equivalent to -vT
        flg->t = 1;
        flg->v = 1;
        break;

      case 'h':
        print_usage(argv[0]);
        exit(0);
        break;

      case '?':
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        ret = 1;
        break;
    }
  }

  // -b имеет приоритет над -n
  if (flg->b) {
    flg->n = 0;
  }

  return ret;
}

void process_char(int c, flg_t *flg, int *line_num, int *empty_count, int *last_char) {
  // Обработка squeeze-blank (-s)
  if (flg->s && c == '\n' && *last_char == '\n') {
    (*empty_count)++;
    if (*empty_count > 1) {
      return;
    }
  } else if (c == '\n') {
    *empty_count = 0;
  } else {
    *empty_count = 0;
  }

  // Нумерация строк (-b и -n)
  if (*last_char == '\n') {
    if ((flg->b && c != '\n') || (flg->n && !flg->b)) {
      printf("%6d\t", (*line_num)++);
    }
  }

  // Обработка отображения символов
  int printed = 0;
  
  // Показ табов (-t или -T)
  if ((flg->t || flg->v) && c == '\t') {
    if (flg->v) {
      printf("^I");
    } else {
      printf("\t");
    }
    printed = 1;
  }
  // Показ концов строк (-e или -E)
  else if ((flg->e || flg->v) && c == '\n') {
    if (flg->v) {
      putchar('$');
    }
    putchar('\n');
    printed = 1;
  }
  // Обработка непечатаемых символов (-v)
  else if (flg->v) {
    if (c == 127) {  // DEL
      printf("^?");
      printed = 1;
    } else if (c < 32 && c != '\t' && c != '\n') {  // Control characters
      printf("^%c", c + 64);
      printed = 1;
    } else if (c > 127 && c < 160) {  // Extended characters
      printf("M-^%c", c - 64);
      printed = 1;
    }
  }

  // Вывод обычного символа если не был обработан специально
  if (!printed) {
    putchar(c);
  }

  *last_char = c;
}

int read_file(const char *filename, flg_t *flg) {
  FILE *file = NULL;
  int ret = 0;

  if (filename == NULL) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
  }

  if (file == NULL) {
    if (filename) {
      fprintf(stderr, "cat: %s: No such file or directory\n", filename);
    } else {
      fprintf(stderr, "cat: cannot open stdin\n");
    }
    return 1;
  }

  int line_num = 1;
  int empty_count = 0;
  int last_char = '\n';  // Начинаем с новой строки для нумерации

  while (1) {
    int c = fgetc(file);
    if (c == EOF) {
      break;
    }
    process_char(c, flg, &line_num, &empty_count, &last_char);
  }

  // Если последний символ не был переводом строки, добавить его
  if (last_char != '\n' && !feof(file)) {
    putchar('\n');
  }

  if (file != stdin) {
    fclose(file);
  }

  return ret;
}

void print_usage(const char *program_name) {
  printf("Usage: %s [OPTION]... [FILE]...\n", program_name);
  printf("Concatenate FILE(s) to standard output.\n\n");
  printf("With no FILE, or when FILE is -, read standard input.\n\n");
  printf("Options:\n");
  printf("  -b, --number-nonblank    number nonempty output lines, overrides -n\n");
  printf("  -e                       equivalent to -vE\n");
  printf("  -E, --show-ends          display $ at end of each line\n");
  printf("  -n, --number             number all output lines\n");
  printf("  -s, --squeeze-blank      suppress repeated empty output lines\n");
  printf("  -t                       equivalent to -vT\n");
  printf("  -T, --show-tabs          display TAB characters as ^I\n");
  printf("  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB\n");
  printf("      --help               display this help and exit\n");
  printf("\nExamples:\n");
  printf("  %s -n file1.txt file2.txt  Number all lines from multiple files\n", program_name);
  printf("  %s -E file.txt             Show $ at end of each line\n", program_name);
}