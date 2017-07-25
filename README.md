# LibEFT

LibEFT is a library implementing Error-Free transformations, meant to be used as
base building blocks for efficient compensated algorithms.

It is originally implemented as a header-only C++ library, but also provides
bindings in C and Fortran90.


## Usage

When building a program using LibEFT (especially in C and Fortran90), you
probably want to enable link-time optimizations in order to give the compiler a
chance to inline calls to Error-Free Transformations.

The recommended command-line compiler options are:

```
-mfma -march=native -D EFT_FMA -flto -ffat-lto-objects
```

In addition, you should use `-O3` or `-Ofast` (but beware of the potential
impact of `-Ofast` on the correctness of floating-point results).

LibEFT uses FMA (Fused Multiplication and Addition) instructions by default, as
it allows better performance. If FMA is not supported by your hardware or you
want to disable it for any reason, remove the `-mfma -D EFT_FMA` options from
the compiler command-line.



### C++

#### Installation

LibEFT is a header-only C++ library. No installation is required; just include
the header:

```c++
#include "libeft.hxx"
```

#### Interface

```c++
template <typename Real>
void fastTwoSum (/* IN  */ const Real &a, const Real &b,
                 /* OUT */ Real &x, Real &e)
```

Compute floating-point numbers `x` and `e` such that `a + b = x + e` and `x =
rnd(a + b)`, using the algorithm from Dekker (1971). It should be good
building-block for compensated summation algorithms such as the `sum2` algorithm
from Ogita, Rump and Oishi (2005).


```c++
template <typename Real>
void twoSum (/* IN  */ const Real &a, const Real &b,
             /* OUT */ Real &x, Real &e)
```

Same as `fastTwoSum`, but using another algorithm. Despite what the names
suggest, `twoSum` should be the fastest of the two.


```c++
template <typename Real>
void twoProd (/* IN  */ const Real &a, const Real &b,
              /* OUT */ Real &x, Real &e)
```

Compute floating-point numbers `x` and `e` such that `a * b = x + e` and `x =
rnd(a * b)`. It internally uses either a FMA operation, or the `twoProd`
algorithm by Neuimaier.



```c++
template <typename Real>
void twoProdSum (/* IN  */ const Real & a, const Real & b, const Real & c,
                 /* OUT */ Real & x, Real & e)
```

Compute floating-point numbers `x` and `e` such that `a * b + c ~ x + e`, where
the equality is only approximate, and `x = rnd(a * b + c)`. This chains calls to
`twoProd` and `twoSum` in the correct order, and is a good building block to
build compensated dot product algorithms like `dot2` from Ogita, Rump and Oishi
(2005).


### C and Fortran90

The provided `Makefile` should build the static library `libeft.a` and the
corresponding Fortran90 module `libeft.mod`.



## License

Copyright (C) 2017 François Févotte, Bruno Lathuilière, EDF SA.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
