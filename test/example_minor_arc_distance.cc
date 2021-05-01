/**
 * @file example_minor_arc_distance.cc
 * @brief test of the `minor_arc.distance()` function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::minor_arc;
using gcxmlib::dcos;
using gcxmlib::degree;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> pos(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p0=(1,0,0) and p1=(0,1,0).\n");
    const dcos p0(1,1,0);
    const dcos p1(1,-1,0);
    const minor_arc arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
    printf("\n\n");
    arc.s.dump();
    arc.e.dump();
    printf("\n\n");
    for (size_t i=0; i<100; i++) {
      const double x(pos(gen)), y(pos(gen)), z(pos(gen)/5.0);
      const dcos p(x,y,z);
      const dcos q = arc.foot_of(p);
      printf("%lf %lf ", arc.separation(p).degree, arc.distance(p).degree);
      printf("%lf %lf %lf ", p.x, p.y, p.z);
      printf("%lf %lf %lf\n", q.x-p.x,q.y-p.y,q.z-p.z);
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
