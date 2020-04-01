all: fileCompressor.c AVL.o minheap.o IO.o huffman.o
	gcc -g -o fileCompressor fileCompressor.c AVL.o minheap.o IO.o huffman.o

AVL.o: AVL.c
	gcc -c AVL.c

fileIO.o: fileIO.c
	gcc -c fileIO.c

minheap.o: minheap.c
	gcc -c minheap.c

IO.o: IO.c
	gcc -c IO.c

huffman.o: huffman.c
	gcc -c huffman.c

clean:
	rm fileCompressor; rm AVL.o; rm minheap.o; rm IO.o; rm huffman.o