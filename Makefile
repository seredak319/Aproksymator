CC = gcc
CFLAGS = -g

aprox4: main.o splines.o points.o own_aprox_4w.o gaus/libge.a
	$(CC) -o MyOwnAprox  main.o splines.o points.o own_aprox_4w.o -L gaus -l ge 

aprox: main.o splines.o points.o aproksymator_na_bazie.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge

aproxDBG: main.o splines.o points.o aproksymator_na_bazie.o gaus/libge.a
	$(CC) $(CFLAGS) -o aproxDBG main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge -ggdb 

intrp: main.o splines.o points.o interpolator.o gaus/libge.a
	$(CC) -o intrp  main.o splines.o points.o interpolator.o -L gaus -l ge

prosta: main.o splines.o points.o prosta.o
	$(CC) -o prosta  main.o splines.o points.o prosta.o	

aproksymator_na_bazie.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_na_bazie.c

interpolator.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c interpolator.c

.PHONY: clean

clean:
	-rm *.o aprox intrp aproxDBG prosta MyOwnAprox
