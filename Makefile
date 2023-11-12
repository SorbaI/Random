CC = gcc
CFLAGS = -Werror -Werror=vla -O2 -std=gnu2x -D_GNU_SOURCE 
all : printrandom clear

printrandom : libRandomSource.so 
	$(CC) main.c $(CFLAGS) -rdynamic -oprintrandom 
libRandomSource.so: random.o linear.o merge.o 
	$(CC) -shared -olibRandomSource.so linear.o random.o merge.o 
random.o:
	$(CC) random.c $(CFLAGS) -fPIC -DPIC -c 
linear.o:
	$(CC) linear.c $(CFLAGS) -fPIC -DPIC -c
merge.o:
	$(CC) merge.c $(CFLAGS) -fPIC -DPIC -c
clear:
	rm linear.o | rm random.o  | rm merge.o