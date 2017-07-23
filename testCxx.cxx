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

  if (argv[1] == std::string("dot")) {
    if (argv[2] == std::string("std"))
      readAndCompute<Dot0>();
    if (argv[2] == std::string("comp"))
      readAndCompute<Dot>();

    return 0;
  }

  return 1;
}
