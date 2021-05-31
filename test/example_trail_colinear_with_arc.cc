/**
 * @file example_trail_colinear_with_arc.cc
 * @brief test of the `trail.colinear_with()` function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::trail;
using gcxmlib::great_circle;
using gcxmlib::minor_arc;
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
    printf("# case 1: p0=(1.0,-0.1,0.0) and p1=(1.0,.1,0.0).\n");
    const timestamp_t t0 = std::chrono::system_clock::now();
    const timestamp_t t1 = t0+seconds(5);
    const footprint p0(1.0,-0.1, 0.0, t0, degree(0.50));
    const footprint p1(1.0, 0.1, 0.0, t1, degree(5.00));
    const trail arc(p0,p1);
    printf("# s : "); arc.s.print();
    printf("# e : "); arc.e.print();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.print();
    printf("\n\n");
    arc.print_error();
    printf("\n\n");
    arc.s.print();
    arc.e.print();
    printf("\n\n");
    size_t count(0);
    for (size_t i=0; i<50000; i++) {
      const double x(pos(gen)), y(pos(gen)), z(pos(gen));
      const double dx(pos(gen)/5.), dy(pos(gen)/5.), dz(pos(gen)/5.);
      const dcos p0(x,y,z), p1(x+dx,y+dy,z+dz);
      const minor_arc m(p0,p1);
      if (arc.colinear_with(m, degree(1.0))) {
        m.s.print(); m.e.print();
        printf("\n");
        count++;
      }
    }
    printf("# %ld arcs are matched.\n", count);
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
