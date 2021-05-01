/**
 * @file example_trail_separation.cc
 * @brief test of the `trail.separation()` function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::trail;
using gcxmlib::dcos;
using gcxmlib::footprint;
using gcxmlib::degree;
using gcxmlib::timestamp_t;
using std::chrono::seconds;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> pos(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p0=(1.0,-0.1,0.0) and p1=(1.0,0.1,0.0).\n");
    const timestamp_t t0 = std::chrono::system_clock::now();
    const timestamp_t t1 = t0+seconds(5);
    const footprint p0(1.0,-0.1, 0.0, t0, degree(0.5));
    const footprint p1(1.0, 0.1, 0.0, t1, degree(3.0));
    const trail arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.dump();
    printf("\n\n");
    arc.dump_error();
    printf("\n\n");
    arc.s.dump();
    arc.e.dump();
    printf("\n\n");
    for (size_t i=0; i<200; i++) {
      const double x(pos(gen)), y(pos(gen)), z(pos(gen));
      const dcos p(x,y,z);
      printf("%lf ", arc.separation(p).degree);
      p.dump();
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
