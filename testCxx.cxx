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
#include <iostream>
#include <iomanip>
#include <vector>
#include <sys/time.h>
#include <string>

double dot0 (const std::vector<double> &x, const std::vector<double> &y) {
  const auto N = x.size();
  double acc = 0;
  for (unsigned long int i = 0 ; i < N ; ++i) {
    acc += x[i] * y[i];
  }
  return acc;
}

double dot (const std::vector<double> &x, const std::vector<double> &y) {
  const auto N = x.size();
  double acc = 0;
  double accErr = 0;
  for (unsigned long int i = 0 ; i < N ; ++i) {
    double err;
    EFT::twoProdSum<double>(x[i], y[i], acc, acc, err);
    accErr += err;
  }
  return acc + accErr;
}



void checkPerf () {
  std::cout << "# N          flops_comp   flops_base" << std::endl;

  volatile unsigned long long int Nmax = 1e8;
  for (unsigned long long int N = 50 ; N < Nmax ; N *= 1.5) {
    std::vector<double> x(N), y(N);
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
      keep += dot(x,y);
    }
    gettimeofday(&end, NULL);
    double elapsed = (end.tv_sec-begin.tv_sec) + 1e-6*(end.tv_usec - begin.tv_usec);


    gettimeofday(&begin, NULL);
    for (unsigned long long int i = 0 ; i < nloops ; ++i) {
      keep += dot0(x,y);
    }
    gettimeofday(&end, NULL);
    double elapsed0 = (end.tv_sec-begin.tv_sec) + 1e-6*(end.tv_usec - begin.tv_usec);

    double flops = (double)nops * (double)nloops / elapsed;
    double flops0 = (double)nops * (double)nloops / elapsed0;
    std::cout << std::scientific << (double)N << " "
              << flops  << " "
              << flops0 << std::endl;
  }
}

template <typename T>
void readAndCompute () {
  unsigned int N;
  std::cin >> N;

  std::vector<double> x(N), y(N);

  for (unsigned int i = 0 ; i<N ; ++i) {
    std::cin >> x[i] >> y[i];
  }

  std::cout << std::setprecision(17) << T::run(x,y) << std::endl;
}

struct Dot {
  static inline double run (const std::vector<double> &x,
                            const std::vector<double> &y) {
    return dot(x,y);
  }
};

struct Dot0 {
  static inline double run (const std::vector<double> &x,
                            const std::vector<double> &y) {
    return dot0(x,y);
  }
};

int main (int argc, char **argv) {

  if (argv[1] == std::string("perf")) {
    checkPerf();
    return 0;
  }
  if (argv[1] == std::string("dot")) {
    if (argv[2] == std::string("std"))
      readAndCompute<Dot0>();
    if (argv[2] == std::string("comp"))
      readAndCompute<Dot>();

    return 0;
  }

  return 1;
}
