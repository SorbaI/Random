CC = gcc
CFLAGS = -O2 -Wall -Werror -std=gnu2x -D_GNU_SOURCE -fsanitize=address,leak,address -fno-sanitize-recover
all : printrandom clear

printrandom : libRandomSource.so main.o
	$(CC) main.o $(CFLAGS) -oprintrandom 
libRandomSource.so: random.o linear.o merge.o
	$(CC) -shared -olibRandomSource.so linear.o random.o merge.o 
random.o:
	$(CC) random.c $(CFLAGS) -fPIC -DPIC -c 
linear.o:
	$(CC) linear.c $(CFLAGS) -fPIC -DPIC -c
merge.o:
	$(CC) merge.c $(CFLAGS) -fPIC -DPIC -c
main.o:
	$(CC) main.c $(CFLAGS) -rdynamic -c
clear:
	rm linear.o | rm random.o  | rm merge.o | rm main.o
