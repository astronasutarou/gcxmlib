/**
 * @file example_minor_arc.cc
 * @brief functional test of the `minor_arc` class.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::minor_arc;
using gcxmlib::footprint;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p0=(1,0,0) and p1=(0,1,0).\n");
    const footprint p0(1,0,0), p1(0,1,0);
    const minor_arc arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
  }

   printf("\n\n");

  {
    printf("# case 2: two random points.\n");
    const footprint p0(unif(gen),unif(gen),unif(gen));
    const footprint p1(unif(gen),unif(gen),unif(gen));
    const minor_arc arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
