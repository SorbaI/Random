#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "random_source.h"

static const char LINEAR[] = "linear";
static const char RANDOM[] = "random";
enum
{
    NUM_BASE = 10,
};
int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Мало аргументов: %d\n", argc);
        exit(1);
    }
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "Ошибка dlopen\n");
        exit(1);
    }
    RandomSource *(*random_factory)(const char *params) = dlsym(handle, "random_random_factory");
    RandomSource *(*linear_factory)(const char *params) = dlsym(handle, "random_linear_factory");
    if (!random_factory & !linear_factory) {
        fprintf(stderr, "Ошибка dlopen\n");
        exit(1);
    }
    RandomSource *src = NULL;
    if (!strcmp(RANDOM, argv[1])) {
        src = random_factory(NULL);
    } else if (!strcmp(LINEAR, argv[1])) {
        if (argc >= 4) {
            src = linear_factory(argv[3]);
        } else {
            fprintf(stderr, "Нет значения seed\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Неизвестный генератор: %s\n", argv[1]);
        exit(1);
    }
    char *endptr = NULL;
    int num = strtoll(argv[2], &endptr, NUM_BASE);
    if (endptr == NULL || errno == ERANGE) {
        fprintf(stderr, "%s: error: %s\n", argv[0], strerror(errno));
        exit(1);
    }
    if (num < 0 || num > 100000) {
        fprintf(stderr, "Недопустимое количество чисел\n");
        exit(1);
    }
    for (int i = 0; i < num; ++i) {
        printf("%.10g\n", src->ops->next(src));
    }
    src->ops->destroy(src);
    return 0;
}
