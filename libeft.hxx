/*
   This file is part of LibEFT, an error-free transformations library
   Parts of this file originally come from Verrou

   Copyright (C) 2014-2017
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

#ifndef HXX_LIBEFT
#define HXX_LIBEFT

#include <functional>
#include <cmath>
#include <xmmintrin.h>


#ifdef EFT_VERROU
#include <valgrind/verrou.h>
#endif // EFT_VERROU

#ifdef EFT_FMA
#include <immintrin.h>
#include <fmaintrin.h>
#endif // EFT_FMA

namespace EFT {

// * Helper functions


// ** Stop/start instrumenting

inline void stopInstr () {
#ifdef EFT_VERROU
  VERROU_STOP_INSTRUMENTATION;
#endif // EFT_VERROU
}

inline void startInstr () {
#ifdef EFT_VERROU
  VERROU_START_INSTRUMENTATION;
#endif // EFT_VERROU
}


#ifdef EFT_FMA
// ** Fused Multiply-Add (FMA) using intrinsics

// return a * b + c
inline double fma (const double& a, const double& b, const double& c){
  double d;
  __m128d ai, bi,ci,di;
  ai = _mm_load_sd(&a);
  bi = _mm_load_sd(&b);
  ci = _mm_load_sd(&c);
  di=_mm_fmadd_sd(ai,bi,ci);
  d=_mm_cvtsd_f64(di);
  return d;
}

inline float fma (const float& a, const float& b, const float& c){
  float d;
  __m128 ai, bi,ci,di;
  ai = _mm_load_ss(&a);
  bi = _mm_load_ss(&b);
  ci = _mm_load_ss(&c);
  di=_mm_fmadd_ss(ai,bi,ci);
  d=_mm_cvtss_f32(di);
  return d;
}
#endif // EFT_FMA

// ** Data types for standard arithmetic operations using intrinsics
// this prevents harmful compiler optimizations where needed

template <typename Real>
class Intrinsic {};

template <>
class Intrinsic<double> {
public:
  Intrinsic (const double &x)
    : _reg (_mm_load_sd(&x))
  {}

  Intrinsic (__m128d reg)
    : _reg (reg)
  {}

  Intrinsic ()
    : _reg()
  {}

  Intrinsic operator- (const Intrinsic& y) const {
    return _mm_sub_sd(_reg, y._reg);
  }

  Intrinsic operator+ (const Intrinsic& y) const {
    return _mm_add_sd(_reg, y._reg);
  }

  Intrinsic operator* (const Intrinsic& y) const {
    return _mm_mul_sd(_reg, y._reg);
  }

  double val () const {
    return _mm_cvtsd_f64 (_reg);
  }

private:
  __m128d _reg;
};

template <>
class Intrinsic<float> {
public:
  Intrinsic (const float &x)
    : _reg (_mm_load_ss(&x))
  {}

  Intrinsic (__m128 reg)
    : _reg (reg)
  {}

  Intrinsic ()
    : _reg()
  {}

  Intrinsic operator- (const Intrinsic& y) const {
    return _mm_sub_ss(_reg, y._reg);
  }

  Intrinsic operator+ (const Intrinsic& y) const {
    return _mm_add_ss(_reg, y._reg);
  }

  Intrinsic operator* (const Intrinsic& y) const {
    return _mm_mul_ss(_reg, y._reg);
  }

  float val () const {
    return _mm_cvtss_f32 (_reg);
  }

private:
  __m128 _reg;
};


// ** Split

// Splitting factor used by split
template<class Real> struct splitFactor {};

// Double precision:  ((2^27)+1), where 27 = sup(53/2)
template<> struct splitFactor<double> {
  enum { value = 134217729 };
};

// Single precision:  ((2^12)+1), where 12 = 24/2
template<> struct splitFactor<float> {
  enum { value = 4097 };
};

// a = x + y
// x = hi(a)
// y = lo(a)
template <typename Real>
static inline void split(const Intrinsic<Real>& ia, Intrinsic<Real>& ix, Intrinsic<Real>& iy){
  typedef const Intrinsic<Real> I;

  stopInstr();
  I ic = I(EFT::splitFactor<Real>::value) * ia;
  ix = ic - (ic-ia);
  iy = ia - ix;
  startInstr();
}




// * Base blocks for compensated algorithms

// ** TwoSum

template <typename Real>
void fastTwoSum (/* IN  */ const Real &a, const Real &b,
                 /* OUT */ Real &x, Real &e) {
  // Fast2Sum algorithm from
  // T. J. Dekker. "A Floating-Point Technique for Extending the Available Precision". 1971
  Real A, B;
  if (std::abs(a) > std::abs(b)) {
    A = a;
    B = b;
  } else {
    A = b;
    B = a;
  }

  typedef const Intrinsic<Real> I;
  I iA(A), iB(B);

  I ix = iA + iB;
  I iz = ix - iA;
  I ie = iB - iz;

  x = ix.val();
  e = ie.val();
}

template <typename Real>
void twoSum (/* IN  */ const Real &a, const Real &b,
             /* OUT */ Real &x, Real &e) {
  typedef const Intrinsic<Real> I;
  I ia(a), ib(b);

  I ix = ia + ib;
  I iz = ix - ia;
  I ie = (ia - (ix-iz)) + (ib-iz);

  x = ix.val();
  e = ie.val();
}


// ** TwoProd

// a * b = x + e
// x = rnd (a * b)
template <typename Real>
void twoProd (/* IN  */ const Real &a, const Real &b,
              /* OUT */ Real &x, Real &e) {
  const Real c = a * b;

#ifdef EFT_FMA

  e = EFT::fma(a, b, -c);

#else

  typedef Intrinsic<Real> I;
  const I ia(a), ib(b), ic(c);
  I ia1, ia2, ib1, ib2;
  EFT::split<Real> (ia, ia1, ia2);
  EFT::split<Real> (ib, ib1, ib2);

  I itmp = (ia1*ib1-ic) + ia1*ib2 + ia2*ib1;
  const Real a2(ia2.val()), b2(ib2.val());
  e = itmp.val() + a2*b2;

#endif

  x = c;
}


// ** TwoProdSum

// a * b + c approx= x + e
template <typename Real>
void twoProdSum (/* IN  */ const Real & a, const Real & b, const Real & c,
                 /* OUT */ Real & x, Real & e) {
  Real ph, pl; EFT::twoProd (a, b, ph, pl);
  Real uh, ul; EFT::twoSum  (c, ph, uh, ul);
#if 0 //def EFT_FMA
  // ErrFmaAppr from
  // S. Boldo, JM. Muller. "Exact and Approximated Error of the FMA". 2011
  x = EFT::fma (a, b, c);

  typedef const Intrinsic<Real> I;
  I ix(x), iuh(uh), iul(ul), ipl(pl);
  I ie = (iuh-ix) + (ipl+iul);
  e = ie.val();
#else
  x = uh;
  e = pl + ul;
#endif // EFT_FMA
}
}

#endif // ndef HXX_LIBEFT
