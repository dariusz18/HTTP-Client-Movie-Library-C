CC=gcc
CFLAGS=-Wall

client: client.o helper.o requests.o parson.o
	$(CC) -o client client.o helper.o requests.o parson.o $(CFLAGS)

clean:
	rm -f *.o client