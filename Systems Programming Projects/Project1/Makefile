all: scannerCSVsorter.out
			echo "Build Done!"
			
scannerCSVsorter.out: scannerCSVsorter.o mergesort.o 
	gcc scannerCSVsorter.o mergesort.o


scannerCSVsorter.o: scannerCSVsorter.c scannerCSVsorter.h 
	gcc -c -Wall scannerCSVsorter.c 

mergesort.o: mergesort.c scannerCSVsorter.h
	gcc -c -Wall mergesort.c
	
clean:
	rm -f *.out *.o

