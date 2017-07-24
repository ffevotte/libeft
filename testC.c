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



void checkPerf () {
  printf ("# N          flops_comp   flops_base\n");

  volatile unsigned long long int Nmax = 1e8;
  for (unsigned long long int N = 50 ; N < Nmax ; N *= 1.5) {
    double *x, *y;
    x = malloc(N*sizeof(double));
    y = malloc(N*sizeof(double));
    volatile double keep = 0;

    for (unsigned long long int i = 0 ; i<N ; ++i) {
      x[i] = 0.1 * i;
      y[i] = 0.01 * i * i;
    }

    unsigned long long int nops = 2*N;
    volatile unsigned long long int nloops = 1e9 / nops;
    if (nloops < 1) {
      nloops = 1;
    }
    struct timeval begin, end;

    gettimeofday(&begin, NULL);
    for (unsigned long long int i = 0 ; i < nloops ; ++i) {
      keep += dot(x,y,N);
    }
    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec-begin.tv_sec) + 1e-6*(end.tv_usec - begin.tv_usec);


    gettimeofday(&begin, NULL);
    for (unsigned long long int i = 0 ; i < nloops ; ++i) {
      keep += dot0(x,y,N);
    }
    gettimeofday(&end, NULL);
    double elapsed0 = (end.tv_sec-begin.tv_sec) + 1e-6*(end.tv_usec - begin.tv_usec);

    double flops = (double)nops * (double)nloops / elapsed;
    double flops0 = (double)nops * (double)nloops / elapsed0;
    printf ("%e %e %e\n", (double)N,
            flops, flops0);
    fflush(stdout);

    free(x);
    free(y);
  }
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

  if (!strcmp (argv[1], "perf")) {
    checkPerf();
    return 0;
  }

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
