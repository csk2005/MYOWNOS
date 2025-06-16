flags=-O2 -Wall -std=c2x
ldflags=

.PHONY: clean

all: clean osapi.o

osapi.o: osapi.c osapi.h os.h
	cc $(flags) $(CFLAGS) -c $<

clean:
	rm -f *.o