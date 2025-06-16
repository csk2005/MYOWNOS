flags=-O2 -Wall -std=c2x -I../osapi
ldflags=

.PHONY: clean clean-osapi

all: clean clean-osapi command.com

../osapi/osapi.o: ../osapi/osapi.c ../osapi/osapi.h
	$(MAKE) -C ../osapi -sk "CFLAGS=$(flags)" 

command.com: command.o ../osapi/osapi.o
	cc $(flags) $^ -o $@ $(ldflags)

command.o: command.c command.h
	cc $(flags) -c $<

clean-osapi:
	cd ../osapi
	$(MAKE) clean

clean:
	rm -f *.o command.com