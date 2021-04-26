/**
 * @file example_source.cc
 * @brief functional test of the source class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "spchord.h"
#include <random>

using spchord::source;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);
  double x(unif(gen)), y(unif(gen)), z(unif(gen));
  source p, q(x,y,z);
  double d = std::sqrt(x*x+y*y+z*z);

  auto s = std::chrono::system_clock::now();

  p.dump();

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
