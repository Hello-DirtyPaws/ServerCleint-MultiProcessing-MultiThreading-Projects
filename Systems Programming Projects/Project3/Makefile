all: bankingServer

bankingClient: bankingClient.o
	gcc bankingClient.o -o bankingClient -pthread -lrt

	echo "Build Done!"
			
bankingServer: bankingServer.o bankingClient
	gcc bankingServer.o -o bankingServer -pthread -lrt

bankingServer.o: bankingServer.c
	gcc -c -Wall bankingServer.c -pthread -lrt

bankingClient.o: bankingClient.c 
	gcc -c -Wall bankingClient.c -pthread -lrt
	
clean:
	rm -f *.o bankingServer bankingClient
