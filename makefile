#make program

#macro:
TARGET = main.o alex.o asin.o ad.o at.o

lftc_exec: $(TARGET)
		gcc -o LFTC_EXEC ${TARGET} -g

main.o: main.c lftc/lftc.h
		gcc -c main.c -g

alex.o: lftc/alex.c lftc/alex.h lftc/lftc.h
		gcc -c lftc/alex.c -g 

asin.o: lftc/asin.c lftc/asin.h lftc/lftc.h
		gcc -c lftc/asin.c -g

ad.o: lftc/ad.c lftc/ad.h lftc/lftc.h
		gcc -c lftc/ad.c -g

at.o: lftc/at.c lftc/at.h lftc/lftc.h
		gcc -c lftc/at.c -g

clean: 
		rm -f *.o

