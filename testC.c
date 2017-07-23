#include "libeft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

double dot0 (double* x, double* y, unsigned long int N) {
  double acc = 0;
  for (unsigned long int i = 0 ; i < N ; ++i) {
    acc += x[i] * y[i];
  }
  return acc;
}

double dot (double* x, double* y, unsigned long int N) {
  double acc = 0;
  double accErr = 0;
  for (unsigned long int i = 0 ; i < N ; ++i) {
    double err;
    twoprodsum_d (x[i], y[i], acc, &acc, &err);
    accErr += err;
  }
  return acc + accErr;
}

void readAndCompute (double (*f)(double*, double*, unsigned long int)) {
  unsigned long int N;
  scanf ("%ld", &N);

  double *x, *y;
  x = malloc (N*sizeof(double));
  y = malloc (N*sizeof(double));

  for (unsigned int i = 0 ; i<N ; ++i) {
    scanf ("%lf %lf", x+i, y+i);
  }

  printf ("%.17e\n", f(x,y, N));
}


int main (int argc, char **argv) {

  if (!strcmp (argv[1], "dot")) {
    if (!strcmp (argv[2], "std")) {
      readAndCompute(dot0);
    }
    if (!strcmp (argv[2], "comp")) {
      readAndCompute(dot);
    }
    return 0;
  }

  return 1;
}
