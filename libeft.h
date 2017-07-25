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

#ifndef H_LIBEFT
#define H_LIBEFT


// * Fortran interface

void twosum_s_ (/* IN  */ float *a, float *b,
                /* OUT */ float *x, float *e);

void twosum_d_ (/* IN  */ double *a, double *b,
                /* OUT */ double *x, double *e);

void twoprod_s_ (/* IN  */ float *a, float *b,
                 /* OUT */ float *x, float *e);

void twoprod_d_ (/* IN  */ double *a, double *b,
                 /* OUT */ double *x, double *e);

void twoprodsum_s_ (/* IN  */ float *a, float *b, float *c,
                    /* OUT */ float *x, float *e);

void twoprodsum_d_ (/* IN  */ double *a, double *b, double *c,
                    /* OUT */ double *x, double *e);


// * C interface

void twosum_s (/* IN  */ float a, float b,
               /* OUT */ float *x, float *e);

void twosum_d (/* IN  */ double a, double b,
               /* OUT */ double *x, double *e);

void twoprod_s (/* IN  */ float a, float b,
                /* OUT */ float *x, float *e);

void twoprod_d (/* IN  */ double a, double b,
                /* OUT */ double *x, double *e);

void twoprodsum_s (/* IN  */ float a, float b, float c,
                   /* OUT */ float *x, float *e);

void twoprodsum_d (/* IN  */ double a, double b, double c,
                   /* OUT */ double *x, double *e);

#endif //ndef H_LIBEFT
