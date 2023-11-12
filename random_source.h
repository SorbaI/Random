#pragma once
#define TYPE_LINEAR 0
#define TYPE_RANDOM 1

typedef struct RandomSource RandomSource;
typedef struct RandomSourceOperations RandomSourceOperations;

struct RandomSource
{
    union
    {
        unsigned long long seed;
        int fd;
    };
    RandomSourceOperations *ops;
    int type;
};

struct RandomSourceOperations
{
    RandomSource *(*destroy)(RandomSource *);
    double (*next)(RandomSource *);
};

extern RandomSource *random_linear_factory(const char *params);
extern RandomSource *random_random_factory(const char *params);
extern RandomSource *destroy(RandomSource *src);
extern double next_random(RandomSource *src);
extern double next_linear(RandomSource *src);
extern double next(RandomSource *src);
