CFLAGS=-Wall -g

all: logfind
	./logfind || true
	./logfind test ./

clean:
	rm -f logfind
