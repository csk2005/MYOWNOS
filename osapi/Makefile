flags=-O2 -Wall -std=c2x -I.
ldflags=
soflags=-shared -fPIC
CFLAGS += -fPIC

.PHONY: clean

all: clean libosapi.so libosapi.a osapi.so diskutil

libosapi.so: osapi.o disk.o omnistd.o fs.o
	cc $(flags) $(ldflags) $(soflags) $^ -o $@

libosapi.a: osapi.o disk.o omnistd.o fs.o
	ar rcs $@ $^

osapi.so: libosapi.so
	ln -sf $< $@

osapi.o: osapi.c osapi.h os.h
	cc $(flags) $(CFLAGS) -c $<

disk.o: disk.c disk.h
	cc $(flags) $(CFLAGS) -c $<

omnistd.o: omnistd.c omnistd.h
	cc $(flags) $(CFLAGS) -c $<

fs.o: fs.c fs.h
	cc $(flags) $(CFLAGS) -c $<

diskutil: diskutil.c osapi.so
	cc $(flags) $(CFLAGS) $^ -o $@
	mv -f diskutil ../util

diskutil.o: diskutil.c
	cc $(flags) $(CFLAGS) -c $<

clean:
	rm -f *.so *.o *.a
