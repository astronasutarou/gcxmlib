/**
 * @file example_trail_propagate.cc
 * @brief test of the `trail.propagate()` function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::trail;
using gcxmlib::minor_arc;
using gcxmlib::dcos;
using gcxmlib::footprint;
using gcxmlib::degree;
using gcxmlib::sec_t;
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
    printf("# case 1: p0=(1.0,-0.1,0.0) and p1=(1.0,.1,0.0).\n");
    const timestamp_t t0 = std::chrono::system_clock::now();
    const timestamp_t t1 = t0+seconds(30);
    const footprint p0(1.0,-0.1, 0.0, t0, degree(0.50));
    const footprint p1(1.0, 0.1, 0.0, t1, degree(1.50));
    const trail arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
    printf("\n\n");
    arc.s.dump();
    arc.e.dump();
    printf("\n\n");
    for (size_t i=0; i<30; i++) {
      const sec_t dt = seconds(10*i);
      const auto q = arc.propagate(dt);
      q.dump();
    }
  }

  printf("\n\n");

  {
    printf("# case 2: two random points.\n");
    const timestamp_t t0 = std::chrono::system_clock::now();
    const timestamp_t t1 = t0+seconds(300);
    const footprint p0(pos(gen),pos(gen),pos(gen),t0,degree(3.50));
    const footprint p1(pos(gen),pos(gen),pos(gen),t1,degree(1.50));
    const trail arc(p0,p1);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.dump();
    printf("\n\n");
    arc.dump_arc();
    printf("\n\n");
    arc.s.dump();
    arc.e.dump();
    printf("\n\n");
    for (size_t i=0; i<30; i++) {
      const timestamp_t t = t1 + seconds(500+10*i);
      const auto q = arc.propagate(t);
      q.dump();
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
