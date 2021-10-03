CC = gcc
CFLAGS = -Wall

all: regServerApp
	
regServerApp: 
	$(CC) $(CFLAGS) swConfServer.c common.c -o swConfServer
	$(CC) $(CFLAGS) swConfClient.c common.c -o swConfClient
			
clean:
	rm *.o swConfServer swConfClient

					
					
