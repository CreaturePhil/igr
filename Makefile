CFLAGS=-Wall -Wextra -pedantic -O2

igr: igr.c
	$(CC) $(CFLAGS) igr.c -g -lcurl -o igr

install:
	mv igr /usr/local/bin

clean:
	rm igr