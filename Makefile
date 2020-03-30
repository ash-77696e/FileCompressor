all: fileCompressor.c AVL.o heap.o IO.o
	gcc -o fileCompressor fileCompressor.c AVL.o heap.o IO.o

AVL.o: AVL.c
	gcc -c AVL.c

fileIO.o: fileIO.c
	gcc -c fileIO.c

heap.o: heap.c
	gcc -c heap.c

IO.o: IO.c
	gcc -c IO.c

clean:
	rm fileCompressor; rm AVL.o; rm heap.o; rm IO.o