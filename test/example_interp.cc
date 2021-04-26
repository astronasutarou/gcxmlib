/**
 * @file example_interp.cc
 * @brief test of the interp() function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::great_circle;
using gcxmlib::minor_arc;
using gcxmlib::dcos;
using gcxmlib::interp;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p1=(1,0,0) and p2=(0,1,0).\n");
    const dcos p1(1,0,0), p2(0,1,0);
    const minor_arc arc(p1,p2);
    arc.dump();
    printf("\n\n");
    for (size_t i=0; i<12; i++) {
       const double phi = 2*M_PI/12*i;
       const auto p = interp(p1,p2,phi);
       p.dump();
    }
  }

   printf("\n\n");

  {
    printf("# case 2: two random points.\n");
    const dcos p1(unif(gen),unif(gen),unif(gen));
    const dcos p2(unif(gen),unif(gen),unif(gen));
    const minor_arc arc(p1,p2);
    arc.dump();
    printf("\n\n");
    for (size_t i=0; i<12; i++) {
      const double phi = 2*M_PI/12*i;
      const auto p = interp(p1,p2,phi);
      p.dump();
    }
  }

  // printf("\n\n");

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
