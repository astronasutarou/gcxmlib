/**
 * @file example_trail_match.cc
 * @brief test of the `trail.match()` function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::trail;
using gcxmlib::great_circle;
using gcxmlib::footprint;
using gcxmlib::degree;
using gcxmlib::timestamp_t;
using std::chrono::seconds;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> pos(-.05,.05);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p0=(1.0,-0.1,0.0) and p1=(1.0,.1,0.0).\n");
    const timestamp_t t0 = std::chrono::system_clock::now();
    const timestamp_t t1 = t0+seconds(100);
    const footprint p0(1.0,-0.1, 0.0, t0, degree(0.50));
    const footprint p1(1.0, 0.1, 0.0, t1, degree(1.00));
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
    size_t count(0);
    for (size_t i=0; i<100; i++) {
      const timestamp_t t0 = arc.s.t + seconds(2000+1000*i);
      const timestamp_t t1 = arc.s.t + seconds(2100+1000*i);
      const footprint pt0 = arc.propagate(t0);
      const footprint pt1 = arc.propagate(t1);
      const double dx0(pos(gen)),dy0(pos(gen)),dz0(pos(gen));
      const double dx1(pos(gen)),dy1(pos(gen)),dz1(pos(gen));
      const footprint p0(pt0.x+dx0,pt0.y+dy0,pt0.z+dz0,t0,pt0.s);
      const footprint p1(pt1.x+dx1,pt1.y+dy1,pt1.z+dz1,t1,pt1.s);
      const trail m(p0,p1);
      const bool match = arc.match(m);
      printf("%ld ", (long)match); m.s.dump();
      printf("%ld ", (long)match); m.e.dump();
      printf("\n");
      count++;
    }
    printf("# %ld arcs are matched.\n", count);
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
