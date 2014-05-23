
sff_demult: main.o sff.o barcode.o
	gcc -g -o sff_demult main.o sff.o barcode.o

main.o: main.c main.h sff.h
	gcc -g -I. -c main.c

sff.o: sff.c sff.h
	gcc -g -I. -c sff.c

barcode.o: barcode.c
	gcc -g -I. -c barcode.c

clean:
	rm sff_demult *.o
