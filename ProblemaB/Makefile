CC=g++
CFLAGS=-Wall -lm

problemaB: problemaB.o
	mkdir -p build/ && \
	$(CC) -o build/problemaB problemaB.o $(CFLAGS)

clean:
	rm build/problemaB build/*.o problemaB *.o