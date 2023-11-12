CC = gcc
CFLAGS = -O2 -Wall -Werror -std=gnu2x -D_GNU_SOURCE -fsanitize=address,leak,address -fno-sanitize-recover
all : printrandom

printrandom : 
	$(CC) main.c random.c linear.c merge.c -rdynamic $(CFLAGS) -oprintrandom 
