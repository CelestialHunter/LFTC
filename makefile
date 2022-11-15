#make program

#macro:
TARGET = main.o alex.o asin.o 

lftc_exec: $(TARGET)
		gcc -o LFTC_EXEC ${TARGET}

main.o: main.c lftc/lftc.h
		gcc -c main.c

alex.o: lftc/alex.c lftc/alex.h
		gcc -c lftc/alex.c 

asin.o: lftc/asin.c lftc/asin.h
		gcc -c lftc/asin.c

clean: 
		rm -f *.o

