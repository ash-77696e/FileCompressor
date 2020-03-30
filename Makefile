all: fileCompressor.c AVL.o
	gcc -o fileCompressor fileCompressor.c AVL.o

AVL.o: AVL.c
	gcc -c AVL.c

fileIO.o: fileIO.c
	gcc -c fileIO.c

clean:
	rm fileCompressor; rm AVL.o