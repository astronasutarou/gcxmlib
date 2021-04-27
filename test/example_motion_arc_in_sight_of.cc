/**
 * @file example_motion_arc_in_sight_of.cc
 * @brief test of the `motion_arc.in_sight_of()` function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::motion_arc;
using gcxmlib::dcos;
using gcxmlib::source;
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
    printf("# case 1: p1=(1,0,0) and p2=(0,1,0).\n");
    const timestamp_t t0 = std::chrono::system_clock::now();
    const timestamp_t t1 = t0+seconds(5);
    const source p1(1.0,-0.1, 0.0,t0,degree(1.00));
    const source p2(1.0, 0.1, 0.0,t1,degree(1.00));
    const motion_arc arc(p1,p2);
    printf("# s : "); arc.s.dump();
    printf("# e : "); arc.e.dump();
    printf("# dt: %lf ms\n", (double)arc.dt.count()*1e3);
    arc.dump();
    printf("\n\n");
    arc.s.dump();
    arc.e.dump();
    printf("\n\n");
    for (size_t i=0; i<10000; i++) {
      const double x(pos(gen)), y(pos(gen)), z(pos(gen));
      const dcos p(x,y,z);
      printf("%d ", (int32_t)(arc.in_sight_of(p)));
      p.dump();
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
