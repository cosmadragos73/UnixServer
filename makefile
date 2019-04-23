
# Makefile for echo client and server
#
EXE 	=	./client/client ./client/client2 ./server/server

SRC 	=	./client/client.c ./client/client2.c ./server/server.c

FLAGS	=	-Wall

all:		$(SRC) socks.o socks.h errlib.o errlib.h
		for f in $(EXE); do (cc $(FLAGS) -o $$f $$f.c socks.o errlib.o); done

sockwrap.o:	socks.c socks.h
		cc $(FLAGS) -c -o socks.o socks.c

errlib.o:	errlib.c errlib.h
		cc $(FLAGS) -c -o errlib.o errlib.c

clean:
		rm -r $(EXE) *.o
