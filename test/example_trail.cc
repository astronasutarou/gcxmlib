/**
 * @file example_trail.cc
 * @brief functional test of the `trail` class.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::trail;
using gcxmlib::source;
using gcxmlib::degree;
using gcxmlib::now;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p0=(1,0,0) and p1=(0,1,0).\n");
    const source p0(1,0,0,now(),degree(0.5));
    const source p1(0,1,0,now(),degree(3.0));
    const trail arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
    printf("\n\n");
    arc.dump_error();
  }

   printf("\n\n");

  {
    printf("# case 2: two random points.\n");
    const source p0(unif(gen),unif(gen),unif(gen),now(),degree(5.0));
    const source p1(unif(gen),unif(gen),unif(gen),now(),degree(1.0));
    const trail arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
    printf("\n\n");
    arc.dump_error();
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
