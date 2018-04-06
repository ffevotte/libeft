# LibEFT

LibEFT is a library implementing Error-Free Transformations (EFTs), meant to be
used as base building blocks for reliable and efficient compensated algorithms
on Intel architectures.

It is originally implemented as a header-only C++ library, but also provides
bindings in C and Fortran90.


## Usage

### Compiler options

When building a program using LibEFT (especially in C and Fortran90), you
probably want to enable link-time optimizations in order to give the compiler a
chance to inline calls to Error-Free Transformations.

The recommended command-line compiler options for `gcc` are:

```
-mfma -march=native -D EFT_FMA -flto -ffat-lto-objects
```

In addition, you should use `-O3` (or `-Ofast`, but beware of the potential
impact of `-Ofast` on the correctness of floating-point results).

#### Without FMA (older architecture)

The recommended options above make LibEFT use FMA (Fused Multiplication and
Addition) instructions, as it allows for better performance. If FMA is not
supported by your hardware or you want to disable it for any reason, remove the
`-mfma -D EFT_FMA` options from the compiler command-line.


#### In conjunction with Verrou

When trying to evaluate FP errors with Monte-Carlo Arithmetic, using a tool like
[Verrou](https://github.com/edf-hpc/verrou), some care must be taken in order
that random roundings don't affect compensated algorithms too much (Graillat,
Jézéquel and Picot, 2018).

Use the `-D EFT_VERROU` command-line switch in order to correctly evaluate FP
errors in LibEFT with Verrou.


### C++

#### Installation

LibEFT is a header-only C++ library. No installation is required; just include
the header:

```c++
#include "libeft.hxx"
```

#### Application Programming Interface (API)

All functions of the API are defined in the `EFT` namespace. They are templated
by a floating-point type, which can be either `float` (for single-precision) or
`double` (for double precision).


&nbsp;

```c++
template <typename Real>
void EFT::fastTwoSum (/* IN  */ const Real &a, const Real &b,
                      /* OUT */ Real &x, Real &e)
```

Compute floating-point numbers *x* and *e* such that *a + b = x + e* and *x =
rnd(a + b)*, using the algorithm by Dekker (1971). It should be a good
building-block for compensated summation algorithms such as the `sum2` algorithm
from Ogita, Rump and Oishi (2005).


&nbsp;

```c++
template <typename Real>
void EFT::twoSum (/* IN  */ const Real &a, const Real &b,
                  /* OUT */ Real &x, Real &e)
```

Same as `fastTwoSum`, but using the algorithm by Knuth (1981). Despite what the
names suggest, `twoSum` should be the fastest of the two.


&nbsp;

```c++
template <typename Real>
void EFT::twoProd (/* IN  */ const Real &a, const Real &b,
                   /* OUT */ Real &x, Real &e)
```

Compute floating-point numbers *x* and *e* such that *a b = x + e* and *x =
rnd(a b)*. It internally uses either a FMA operation (if the `EFT_FMA` macro is
defined), or the `twoProd` algorithm by Neumaier (1974).


&nbsp;

```c++
template <typename Real>
void EFT::twoProdSum (/* IN  */ const Real & a, const Real & b, const Real & c,
                      /* OUT */ Real & x, Real & e)
```

Compute floating-point numbers *x* and *e* such that *a b + c ~ x + e*, where
the equality is only approximate, and *x = rnd(a b + c)*. This chains calls to
`twoProd` and `twoSum` in the correct order, and is a good building block to
build compensated dot product algorithms like `dot2` from Ogita, Rump and Oishi
(2005).


### C and Fortran90

The provided `Makefile` should build the static library `libeft.a` and the
corresponding Fortran90 module `libeft.mod`.


The provided interface is the same as in C++, except that ther is no namespace,
and function names are all-lowercase and respectively suffixed with `_s` and
`_d` for the single- and double-precision versions.

For example, the C wrapper around `EFT::twoSum<float>` is called `twosum_s`.

Files `testC.c` and `testF.f90` can be used as examples of how to implement a
compensated dot product in C and Fortran90 using LibEFT.


## Example: quality and performance of a dot product

File `testCxx.cxx` implements two dot product algorithms: a standard one
(native) and a compensated one (comp) using LibEFT. It can be used as an example
of how to use the library.

The quality of the results is shown in the following figure, where the relative
error of the dot product result (in double precision) is shown as a function of
the condition number of the problem (following the methodology described in
Ogita, Rump and Oishi, 2005). We see that this compensated dot product
implementation approximately reaches the accuracy that would be obtained with the
quadruple precision implementation of a standard dot product algorithm.

<img src="https://github.com/ffevotte/libeft/raw/gh-pages/qual.svg?raw=true&sanitize=true"
     style="width:100%"/>


The performance reached by this implementation is summarized in the figures
below, with and without FMA. As expected, the compensated implementation is
always CPU-bound: its performance does not depend on the size of the
computation. This is in contrast to the standard implementation, which is
essentially memory-bound, and whose performance drops for vectors larger than
10<sup>5</sup> or 10<sup>6</sup> elements.

The performance drop due to compensation thus varies between 15-25 (for small
vectors) to around 5 (for large vectors). The FMA version is more efficient overall.

<img src="https://github.com/ffevotte/libeft/raw/gh-pages/perf-fma.svg?raw=true&sanitize=true"
     style="width:50%"/>
<img src="https://github.com/ffevotte/libeft/raw/gh-pages/perf-nofma.svg?raw=true&sanitize=true"
     style="width:50%"/>


## Contributing

If you make improvements to this code or have suggestions, please do not
hesitate to fork the repository or submit bug reports on
[github](https://github.com/ffevotte/libeft). The repository's URL is:

    https://github.com/ffevotte/libeft.git


## License

Copyright (C) 2017-2018 François Févotte, Bruno Lathuilière, EDF SA.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
