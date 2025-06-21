flags=-O2 -Wall -std=c2x -I.
ldflags=
soflags=-shared -fPIC

.PHONY: clean

all: clean osapi.so

osapi.so: osapi.o disk.o
	cc $(flags) $(ldflags) $(soflags) $^ -o $@

osapi.o: osapi.c osapi.h os.h
	cc $(flags) $(CFLAGS) -c $<

disk.o: disk.c disk.h
	cc $(flags) $(CFLAGS) -c $<

clean:
	rm -f *.so *.o