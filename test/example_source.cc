/**
 * @file example_source.cc
 * @brief functional test of the source class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "spchord.h"
#include <random>
#include <unistd.h>

using spchord::source;
using std::chrono::seconds;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);
  std::uniform_real_distribution<double> sleep(0,10000);

  auto s = std::chrono::system_clock::now();

  for (size_t i=0; i<100; i++) {
    {
      const spchord::timestamp_t t0 = std::chrono::system_clock::now();
      double wait(sleep(gen));
      double x(unif(gen)), y(unif(gen)), z(unif(gen));
      source p(x,y,z,t0+seconds(i));
      p.dump();
      usleep(wait);
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
