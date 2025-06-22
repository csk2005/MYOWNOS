flags=-O2 -Wall -std=c2x -I../osapi
ldflags=-Wl,-rpath,'$$ORIGIN/../osapi'

.PHONY: clean clean-osapi

all: clean clean-osapi command.com

../osapi/libosapi.so ../osapi/libosapi.a: ../osapi/osapi.c ../osapi/osapi.h ../osapi/disk.c ../osapi/disk.h
	$(MAKE) -C ../osapi "CFLAGS=$(flags) -fPIC"

command.com: command.o ../osapi/libosapi.so ../osapi/libosapi.a
	cc $(flags) $(ldflags) command.o -L../osapi -losapi -o $@

command.o: command.c command.h
	cc $(flags) -c $<

clean-osapi:
	$(MAKE) -C ../osapi clean

clean:
	rm -f *.o command.com
