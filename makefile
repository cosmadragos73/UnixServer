
# Makefile for echo client and server
#
#EXE 	= 
#./server/udp/Server ./server/udptcp/Server ./server/tcp/Server ./server/tcp2/Server
#./client/udp/Client ./client/udptcp/Client ./client/tcp/Client ./client/tcp2/Client
EXE_SERVER_TCP = ./server/tcp/Server  #./server/tcp2/Server
EXE_SERVER_UDP = ./server/udp/Server  

EXE_CLIENT_UDP = ./client/udp/Client 
EXE_CLIENT_TCP = ./client/tcp/Client #./client/tcp2/Client

EXE_CLIENT_TCP_UDP = ./client/udptcp/Client
EXE_SERVER_TCP_UDP = ./server/udptcp/Server

SRC_SERVER_UDP = ./server/udp/Server.c
SRC_CLIENT_UDP = ./client/udp/Client.c 

SRC_SERVER_TCP = ./server/tcp/Server.c #./server/tcp2/Server.c
SRC_CLIENT_TCP = ./client/tcp/Client.c #./client/tcp2/Client.c

SRC_SERVER_TCP_UDP = ./server/udptcp/Server.c
SRC_CLIENT_TCP_UDP = ./client/udptcp/Client.c 

FLAGS	=	-Wall

client_tcp: $(SRC_CLIENT_TCP) 
		for f in $(EXE_CLIENT_TCP); do (cc $(FLAGS) -o $$f $$f.c ); done

client_udp: $(SRC_CLIENT_UDP) utils/socks.o utils/socks.h utils/errlib.o utils/errlib.h
		for f in $(EXE_CLIENT_UDP); do (cc $(FLAGS) -o $$f $$f.c utils/socks.o utils/errlib.o); done

client_udp_tcp:$(SRC_CLIENT_TCP_UDP) 
		for f in $(EXE_CLIENT_TCP_UDP); do (cc $(FLAGS) -o $$f $$f.c ); done


server_tcp:$(SRC_SERVER_TCP) 
		for f in $(EXE_SERVER_TCP); do (cc $(FLAGS) -o $$f $$f.c ); done

server_udp:$(SRC_SERVER_UDP) utils/socks.o utils/socks.h utils/errlib.o utils/errlib.h
		for f in $(EXE_SERVER_UDP); do (cc $(FLAGS) -o $$f $$f.c utils/socks.o utils/errlib.o); done

server_udp_tcp:$(SRC_SERVER_TCP_UDP) 
		for f in $(EXE_SERVER_TCP_UDP); do (cc $(FLAGS) -o $$f $$f.c ); done

#client: $(SRC_CLIENT) utils/socks.o utils/socks.h utils/errlib.o utils/errlib.h
#		for f in $(EXE_CLIENT); do (cc $(FLAGS) -o $$f $$f.c utils/socks.o utils/errlib.o); done

#server: $(SRC_SERVER) utils/socks.o utils/socks.h utils/errlib.o utils/errlib.h
#		for f in $(EXE_SERVER); do (cc $(FLAGS) -o $$f $$f.c utils/socks.o utils/errlib.o); done
#all:		$(SRC) utils/socks.o utils/socks.h utils/errlib.o utils/errlib.h
#		for f in $(EXE); do (cc $(FLAGS) -o $$f $$f.c utils/socks.o utils/errlib.o); done

sockwrap.o:	utils/socks.c utils/socks.h
		cc $(FLAGS) -c -o socks.o socks.c

errlib.o:	utils/errlib.c utils/errlib.h
		cc $(FLAGS) -c -o errlib.o errlib.c

clean:
		rm -r $(EXE) *.o
