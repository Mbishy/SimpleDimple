#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int opt;
    
    printf("До парсинга: optind = %d\n", optind);
    
    while ((opt = getopt(argc, argv, "abc:")) != -1) {
        printf("Обрабатываем опцию: %c, optind = %d\n", opt, optind);
        switch (opt) {
            case 'a': printf("Опция -a\n"); break;
            case 'b': printf("Опция -b\n"); break;
            case 'c': printf("Опция -c с аргументом: %s\n", optarg); break;
        }
    }
    
    printf("После парсинга: optind = %d\n", optind);
    
    // Обработка оставшихся аргументов (файлов)
    for (int i = optind; i < argc; i++) {
        printf("Файл: %s\n", argv[i]);
    }
    
    return 0;
}