#include "splines.h"

#include <stdlib.h>

#define MALLOC_FAILED( P, SIZE ) (((P)=malloc( (SIZE)*sizeof( *(P))))==NULL)

int
alloc_spl (spline_t * spl, int n)
{
  spl->n = n;
  return MALLOC_FAILED (spl->x, spl->n); // wystarczy nam jedna tablica o rozmiarze n czyli nb = 5 na wspolczynniki a0,a1...a4;
/*  || MALLOC_FAILED (spl->f, spl->n)
    || MALLOC_FAILED (spl->f1, spl->n)
    || MALLOC_FAILED (spl->f2, spl->n)
    || MALLOC_FAILED (spl->f3, spl->n);*/
}

int
read_spl (FILE * inf, spline_t * spl)
{
  int i;
  if (fscanf (inf, "%d", &(spl->n)) != 1 || spl->n < 0)
    return 1;

  if (alloc_spl (spl, spl->n))
    return 1;

  for (i = 0; i < spl->n; i++)
    if (fscanf
        (inf, "%lf", spl->x + i/*, spl->f + i, spl->f1 + i, // nie mamy paru pochodnych tylko wspolczynniki a0, a1, .., a4;
         spl->f2 + i, spl->f3 + i*/) != 1)
      return 1;

  return 0;
}

void
write_spl (spline_t * spl, FILE * ouf)
{
  int i;
  fprintf (ouf, "%d\n", spl->n);
  for (i = 0; i < spl->n; i++)
    fprintf (ouf, "%g\n", spl->x[i] /* , spl->f[i], spl->f1[i], // wypisujemy tylko jedna tablice jest nia x;
             spl->f2[i], spl->f3[i]*/);
}

double
value_spl (spline_t * spl, double x)
{

/*
  int i;
  double dx;

  for (i = spl->n - 1; i > 0; i--)
    if (spl->x[i] < x)
      break;

  dx = x - spl->x[i];

  return spl->f[i]
	+ dx * spl->f1[i]
	+ dx * dx / 2 *  spl->f2[i] 
	+ dx * dx * dx / 6 * spl->f3[i];

*/
	double temp = 1;
	double result = 0;
	int i;
	
	result += spl->x[0]; // a4 - wyraz wolny wielomianu

// petle for symbolizuja odpowiednia potege x'a;

	for(i=0; i<1; i++)  // a3  
	temp *= x;
	temp *= spl->x[1];
	result += temp;
	temp =1;
	
	for(i=0; i<2; i++)  // a2
	temp *= x;
	temp *= spl->x[2];
	result += temp;
	temp =1;
	
	
	
	for(i=0; i<3; i++)  // a1
	temp *= x;
	temp *= spl->x[3];
	result += temp;
	temp =1;
	
	for(i=0; i<4; i++) // a0 - wyraz przy x^4
	temp *= x;
	temp *= spl->x[4];
	result += temp;


return result; 


}

