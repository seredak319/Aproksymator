#include "makespl.h"

#include "gaus/piv_ge_solver.h"

#define APROX_BASE_SIZE 5

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//    APROKSYMACJA SREDNIOKWADRATOWA Z BAZA WIELOMIANOW 4 STOPNIA 


double valA(double* x, int j, int i, int np)
{
	double wynik=0;
	int power = i+j;
	double value=1;

	if(power == 0){
	wynik = np;
	return wynik;
	}

	for( int k = 0; k< np; k++)
	{
		value=1;	
		for(int z=0; z<power; z++)
		value *= *(x + k);

	wynik += value;
	}


return wynik;
}


double valB(double* x, double* y, int j, int np)
{
	double wynik=0;
	int power = j;
	double value=1;

	if(power == 0)
	{
	  for(int k = 0; k<np; k++)
	  {
		  value=1;
		value *= *(y + k);
	wynik += value;	
	  }
	  return wynik;
	}

	for( int k = 0; k< np; k++)
	{
		value=1;
		for(int z=0; z<power; z++)
		value *= *(x + k);

	value *= *(y + k);
	
	wynik += value;
	}

return wynik;
}
		 			
			

void make_spl(points_t *pts, spline_t *spl)
{
	matrix_t *eqs = NULL;
	double *x = pts->x;	
	double *y = pts->y;

	int i,j,k;
	int np = pts->n;
	int nb=5;

	char *nbEnv = getenv("APROX_BASE_SIZE"); //zdefiniowana jako 5, poniewaz baza to wielomian 4. st. 

	if( nbEnv != NULL && atoi( nbEnv ) > 0 ) 
		 nb = atoi( nbEnv );

	eqs = make_matrix(nb,nb+1);
	
	//	AllValuesOfLeftSide5x5[9]; // n, Exi^1, Exi^2, ... Exi^8  // lewa strona wszystkie ewentualne mozliwosci
	//	AllValuesOfRightSide5x1[5]; // Eyi, Eyixi, Eyixi^2, Eyixi^3, Eyixi^4 // prawa strona wszystkie ewentualne mozliwosci


	for(j=0; j<nb;j++) // wiersze
	{
		for(i=0;i<nb; i++) //kolumny
			add_to_entry_matrix(eqs,j,i,valA(x,j,i,np)); 

			add_to_entry_matrix(eqs,j,nb,valB(x,y,j,np));

	}
#ifdef DEBUG // macierz przed rozwiazaniem
	write_matrix(eqs, stdout);
#endif
	if(piv_ge_solver(eqs))
	{
		spl->n = 0;
		return;
	}

/* piv_ge_solver utworzy macierz trojkatna dolna (biorac pod uwage macierz 5x5) uwzgledniajac rowniez macierz prawych stron
 * gdyz przy deklarowaniu make_matrix(5,6) wartosc 6 odpowiada eqs-> cn; Zatem iteracje w piv_ge_in_situ_matrix(eqs) dotyczyc rowniez beda wektora prawych stron.
 * Funkcja bs_matrix(eqs) natomiast wyznaczy niewiadome (a0,a1,a2,a3,a4) w miejscu wektora prawych stron poprzez podstawienie wsteczne.
 */

#ifdef DEBUG // macierz po rozwiazaniu
	write_matrix(eqs,stdout);
#endif
	if( alloc_spl(spl,nb) == 0 )
		for(i = 0; i < nb; i++) 
		spl->x[i] = get_entry_matrix(eqs, i, nb);

#ifdef DEBUG
	
	FILE *dbg = fopen("log_MyOwnAprox.txt", "w");

	for(i = 0; i < nb; i++) 
	fprintf(dbg, "\t DEBUG INFO spl->x[%d] =: %lf\n",i, spl->x[i]);

	fclose(dbg);
#endif
		
		
}


