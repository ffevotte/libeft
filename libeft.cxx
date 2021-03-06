/*
   This file is part of LibEFT, an error-free transformations library

   Copyright (C) 2017
     F. Févotte     <francois.fevotte@edf.fr>
     B. Lathuilière <bruno.lathuiliere@edf.fr>
     EDF SA

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

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
