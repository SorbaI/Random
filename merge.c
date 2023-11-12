#include "random_source.h"
#include <stdlib.h>
#include <unistd.h>

double
next(RandomSource *src)
{
    if (src->type == TYPE_LINEAR) {
        return next_linear(src);
    }
    return next_random(src);
}

RandomSource *
destroy(RandomSource *src)
{
    if (src != NULL) {
        if (src->type == TYPE_RANDOM) {
            close(src->fd);
        }
        if (src->ops != NULL) {
            free(src->ops);
        }
        free(src);
    }
    return NULL;
}
