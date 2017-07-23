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
