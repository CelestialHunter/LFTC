#make program

#macro:
TARGET = main.o alex.o asin.o

lftc_exec: $(TARGET)
		gcc -o LFTC_EXEC ${TARGET} -g

main.o: main.c lftc/lftc.h
		gcc -c main.c -g

alex.o: lftc/alex.c lftc/alex.h lftc/lftc.h
		gcc -c lftc/alex.c -g 

asin.o: lftc/asin.c lftc/asin.h lftc/lftc.h
		gcc -c lftc/asin.c -g

clean: 
		rm -f *.o

