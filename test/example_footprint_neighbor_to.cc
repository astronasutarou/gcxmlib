/**
 * @file example_footprint_neighbor_to.cc
 * @brief test of the footprint.neighbor_to() function
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

  footprint p0(0,0,1);

  for (size_t i=0; i<10000; i++) {
    {
      const gcxmlib::timestamp_t t0 = gcxmlib::now();
      double x(unif(gen)), y(unif(gen)), z(1.0);
      footprint p(x,y,z,t0+seconds(i));
      if (p0.neighbor_to(p, degree(5.0))) {
        printf("1 ");
      } else {
        printf("0 ");
      }
      p.print();
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  printf("# 10000 random footprints are generated.\n");
  printf("# [tag, x, y, z, t] are listed.\n");
  printf("# the first element `tag` becomes unity,\n"
         "# if footprint is located within 5 degree of (0,0,1).\n");
  return 0;
}
