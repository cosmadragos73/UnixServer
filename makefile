
# Makefile for echo client and server
#
#EXE 	=	./client/udp/client ./server/udp/server ./client/tcp/client ./server/tcp/server
EXE 	= ./server/udp/server ./client/udp/client
 
#SRC 	=	./client/udp/client.c ./server/udp/server.c ./client/tcp/client.c ./server/tcp/server.c
SRC 	= ./server/udp/server.c ./client/udp/client.c
	
FLAGS	=	-Wall

all:		$(SRC) utils/socks.o utils/socks.h errlib.o utils/errlib.h
		for f in $(EXE); do (cc $(FLAGS) -o $$f $$f.c socks.o errlib.o); done

sockwrap.o:	utils/socks.c utils/socks.h
		cc $(FLAGS) -c -o socks.o socks.c

errlib.o:	utils/errlib.c utils/errlib.h
		cc $(FLAGS) -c -o errlib.o errlib.c

clean:
		rm -r $(EXE) *.o
