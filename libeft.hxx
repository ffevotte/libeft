#ifndef HXX_LIBEFT
#define HXX_LIBEFT

#include <functional>
#include <cmath>

#ifdef EFT_FMA
#include <immintrin.h>
#include <fmaintrin.h>
#endif // EFT_FMA

namespace EFT {

#pragma GCC push_options
#pragma GCC optimize ("O3")

// * Helper functions

#ifdef EFT_FMA
// ** Fused Multiply-Add (FMA)

// return a * b + c
double fma (const double& a, const double& b, const double& c){
  double d;
  __m128d ai, bi,ci,di;
  ai = _mm_load_sd(&a);
  bi = _mm_load_sd(&b);
  ci = _mm_load_sd(&c);
  di=_mm_fmadd_sd(ai,bi,ci);
  d=_mm_cvtsd_f64(di);
  return d;
}

float fma (const float& a, const float& b, const float& c){
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


// ** Split

// Splitting factor used by split
template<class Real> constexpr const Real splitFactor = 0. / 0.; // NaN

// Double precision:  ((2^27)+1), where 27 = sup(53/2)
template<> constexpr const double splitFactor<double> = 134217729;

// Single precision:  ((2^12)+1), where 12 = 24/2
template<> constexpr const float splitFactor<float> = 4097;

// a = x + y
// x = hi(a)
// y = lo(a)
template <typename Real>
static inline void split(Real a, Real& x, Real& y){
  const Real c = EFT::splitFactor<Real> * a;
  x = c - (c-a);
  y = a - x;
}




// * Base blocks for compensated algorithms

// ** TwoSum

template <typename Real>
void twoSum (/* IN  */ const Real &a, const Real &b,
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
  x = A + B;
  const Real z = x - A;
  e = B - z;
}


// ** TwoProd

// a * b = x + e
// x = rnd (a * b)
template <typename Real>
void twoProd (/* IN  */ const Real &a, const Real &b,
              /* OUT */ Real &x, Real &e) {
  x = a * b;
#ifdef EFT_FMA
  e = EFT::fma(a, b, -x);
#else
  Real a1,a2;
  Real b1,b2;
  EFT::split<Real> (a, a1, a2);
  EFT::split<Real> (b, b1, b2);
  e = ((a1*b1-x) + a1*b2 + a2*b1) + a2*b2;
#endif
}


// ** TwoProdSum

// a * b + c approx= x + e
template <typename Real>
void twoProdSum (/* IN  */ const Real & a, const Real & b, const Real & c,
                 /* OUT */ Real & x, Real & e) {
  Real ph, pl; EFT::twoProd (a, b, ph, pl);
  Real uh, ul; EFT::twoSum  (c, ph, uh, ul);
#ifdef EFT_FMA
  // ErrFmaAppr from
  // S. Boldo, JM. Muller. "Exact and Approximated Error of the FMA". 2011
  x = EFT::fma (a, b, c);
  e = (uh-x) + (pl+ul);
#else
  x = uh;
  e = pl + ul;
#endif // EFT_FMA
}

#pragma GCC pop_options
}

#endif // ndef HXX_LIBEFT
