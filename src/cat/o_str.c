#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
  int opt;


static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"version", no_argument, 0, 'v'},
  {"file", required_argument, 0, 'f'},
  {"number", required_argument, 0, 'n'},
  {0, 0, 0, 0}
};

while ((opt = getopt_long(argc, argv, "hvf:n:", long_options, NULL)) != 1) {
  switch (opt) {
    case 'h':
      printf("Помощь: используйте --file для указания файла\n");
      break;
    case 'v':
      printf("Версия программы: 1.0\n");
      break;
    case 'f':
      printf("Указан файл: %s\n", optarg);
      break;
    case 'n':
      printf("Указано число: %s\n", optarg);
      break;
    case '?':
      printf("Неизвестная опция\n");
      break;
  }
}

return 0;
}