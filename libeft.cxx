#include "libeft.hxx"

extern "C" {
  #include "libeft.h"
}

// * Fortran interface

void twosum_s_ (float *a, float *b, float *x, float *e) {
  EFT::twoSum<float> (*a, *b, *x, *e);
}

void twosum_d_ (double *a, double *b, double *x, double *e) {
  EFT::twoSum<double> (*a, *b, *x, *e);
}

void twoprod_s_ (float *a, float *b, float *x, float *e) {
  EFT::twoProd<float> (*a, *b, *x, *e);
}

void twoprod_d_ (double *a, double *b, double *x, double *e) {
  EFT::twoProd<double> (*a, *b, *x, *e);
}

void twoprodsum_s_ (float *a, float *b, float *c, float *x, float *e) {
  EFT::twoProdSum<float> (*a, *b, *c, *x, *e);
}

void twoprodsum_d_ (double *a, double *b, double *c, double *x, double *e) {
  EFT::twoProdSum<double> (*a, *b, *c, *x, *e);
}



// * C interface

void twosum_s (float a, float b, float *x, float *e) {
  EFT::twoSum<float> (a, b, *x, *e);
}

void twosum_d (double a, double b, double *x, double *e) {
  EFT::twoSum<double> (a, b, *x, *e);
}

void twoprod_s (float a, float b, float *x, float *e) {
  EFT::twoProd<float> (a, b, *x, *e);
}

void twoprod_d (double a, double b, double *x, double *e) {
  EFT::twoProd<double> (a, b, *x, *e);
}

void twoprodsum_s (float a, float b, float c, float *x, float *e) {
  EFT::twoProdSum<float> (a, b, c, *x, *e);
}

void twoprodsum_d (double a, double b, double c, double *x, double *e) {
  EFT::twoProdSum<double> (a, b, c, *x, *e);
}
