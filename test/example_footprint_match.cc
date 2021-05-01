/**
 * @file example_footprint_match.cc
 * @brief test of the footprint.match() function
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::footprint;
using gcxmlib::degree;
using std::chrono::seconds;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-0.3,0.3);

  auto s = std::chrono::system_clock::now();

  const gcxmlib::timestamp_t t0 = gcxmlib::now();
  footprint p0(0,0,1,t0+seconds(5000));
  gcxmlib::angle dR = degree(10.0);
  gcxmlib::sec_t dT(1000.0);

  for (size_t i=0; i<10000; i++) {
    {
      const gcxmlib::timestamp_t t = gcxmlib::now();
      double x(unif(gen)), y(unif(gen)), z(1.0);
      footprint p(x,y,z,t+seconds(i));
      if (p0.match(p, dR, dT)) {
        printf("1 ");
      } else {
        printf("0 ");
      }
      p.dump();
    }
  }
  printf("1 "); p0.dump();

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  printf("# 10000 random footprints are generated.\n");
  printf("# [tag, x, y, z, t] are listed.\n");
  printf("# the first element `tag` becomes unity,\n"
         "# if footprint is located within 10 degree of (0,0,1)"
         "# and timestamp is matched within 1000 seconds.\n");
  return 0;
}
