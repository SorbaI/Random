#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "random_source.h"

static const char program_name[] = "random.c";
static const unsigned long long MUL = 1103515245;
static const unsigned long long INC = 12345;
static const unsigned long long MOD = ((unsigned) 1 << 31);
static const int NUM_BASE = 10;

RandomSource *
random_linear_factory(const char *params)
{
    char *endptr = NULL;
    unsigned long long seed = strtoull(params, &endptr, NUM_BASE);
    if (endptr == NULL || errno == ERANGE) {
        fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
        exit(1);
    }
    RandomSource *item = calloc(1, sizeof(*item));
    if (item == NULL) {
        fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
        exit(1);
    }
    item->type = TYPE_LINEAR;
    item->seed = seed & (MOD - 1);
    RandomSourceOperations *ops = calloc(1, sizeof(*ops));
    if (ops == NULL) {
        fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
        exit(1);
    }
    item->ops = ops;
    item->ops->next = next;
    item->ops->destroy = destroy;
    return item;
}

double
next_linear(RandomSource *src)
{
    src->seed = (src->seed * MUL + INC) % MOD;
    return (src->seed) / (double) MOD;
}
