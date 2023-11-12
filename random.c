#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "random_source.h"

static const char program_name[] = "random.c";

RandomSource *
random_random_factory(const char *params)
{
    RandomSource *item = calloc(1, sizeof(*item));
    if (item == NULL) {
        fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
        exit(1);
    }
    item->type = TYPE_RANDOM;
    RandomSourceOperations *ops = calloc(1, sizeof(*ops));
    if (ops == NULL) {
        fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
        exit(1);
    }
    item->ops = ops;
    item->ops->next = next;
    item->ops->destroy = destroy;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
        exit(1);
    }
    item->fd = fd;
    return item;
}

double
next_random(RandomSource *src)
{
    unsigned long long ans = 0;
    unsigned char buf[sizeof(ans)];
    int i = 0;
    while (i < sizeof(ans)) {
        int res = read(src->fd, &buf[i], sizeof(ans) - i);
        if (res < 0) {
            fprintf(stderr, "%s: error: %s\n", program_name, strerror(errno));
            exit(1);
        }
        i += res;
    }
    for (int i = 0, j = 0; i < sizeof(ans); i++, j += CHAR_BIT) {
        ans |= (unsigned long long) buf[i] << j;
    }
    return ans / (double) ULLONG_MAX;
}
