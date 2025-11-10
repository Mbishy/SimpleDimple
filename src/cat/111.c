#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// Определяем структуру для хранения флагов аргументов
// Это похоже на флаги cat команды в Linux
typedef struct arguments {
  int b, n, s, E, e, T, t, v;  // Флаги различных опций
} arguments;

// Функция для разбора аргументов командной строки
arguments argument_parser(int argc, char **argv) {
  arguments arg = {0};  // Инициализируем ВСЕ флаги нулями

  // Массив длинных опций (с двумя дефисами)
  struct option long_options[] = {
      {"number", no_argument, NULL, 'n'},           // --number
      {"number-nonblank", no_argument, NULL, 'b'},  // --number-nonblank  
      {"squeeze-blank", no_argument, NULL, 's'},    // --squeeze-blank
      {0, 0, 0, 0}  // Маркер конца массива (ОБЯЗАТЕЛЬНО!)
  };

  // ПРОБЛЕМА: getopt_long вызывается только ОДИН раз!
  // Он должен быть в цикле while для обработки всех опций
  int opt;
  opt = getopt_long(argc, argv, "bnEeTts", long_options, 0);
  
  // Обрабатываем только ПЕРВУЮ найденную опцию
  switch (opt) {
    case 'b':  // Опция --number-nonblank или -b
      arg.b = 1;  // Нумеровать только непустые строки
      break;

    case 'n':  // Опция --number или -n  
      arg.n = 1;  // Нумеровать все строки
      break;

    case 'e':  // Опция -e
      arg.E = 1;  // Показывать символы конца строки как $
      arg.v = 1;  // Включить отображение непечатаемых символов
      break;

    case 's':  // Опция --squeeze-blank или -s
      arg.s = 1;  // Сжимать несколько пустых строк в одну
      break;

    case 'E':  // Опция -E (аналог -e но без -v)
      arg.e = 1;  // Показывать символы конца строки
      break;

    case 'T':  // Опция -T (аналог -t но без -v)
      arg.T = 1;  // Показывать табуляции как ^I
      break;

    case 't':  // Опция -t
      arg.t = 1;  // Показывать табуляции и другие спецсимволы
      break;

    case '?':  // Неизвестная опция
      perror("ERROR");
      exit(1);
      break;

    default:   // Другие случаи (часто -1 когда опций нет)
      // Закомментировано, чтобы не завершать программу
      //perror("ERROR");
      //exit(1);
      ;  // Пустая операция
      break;
  }
  
  // ПРОБЛЕМА: функция не возвращает значение!
  // Должен быть: return arg;
}







/*Исправленная версия*/

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
        fprintf(stderr, "Неизвестная опция\n");
        exit(1);
      default:
        fprintf(stderr, "Ошибка разбора опций\n");
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