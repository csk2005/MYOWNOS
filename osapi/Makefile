flags=-O2 -Wall -std=c2x -I.
ldflags=
soflags=-shared -fPIC
CFLAGS += -fPIC

.PHONY: clean

all: clean libosapi.so

libosapi.so: osapi.o disk.o
	       cc $(flags) $(ldflags) $(soflags) $^ -o $@

osapi.so: libosapi.so
	ln -sf $< $@

osapi.o: osapi.c osapi.h os.h
	cc $(flags) $(CFLAGS) -c $<

disk.o: disk.c disk.h
	cc $(flags) $(CFLAGS) -c $<

clean:
	rm -f *.so *.o
