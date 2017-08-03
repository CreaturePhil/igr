CFLAGS=-Wall -Wextra -pedantic

igr: igr.c
	$(CC) $(CFLAGS) igr.c -g -lcurl -o igr

install:
	mv igr /usr/local/bin

clean:
	rm igr