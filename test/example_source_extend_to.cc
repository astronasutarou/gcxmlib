/**
 * @file example_source_extend_to.cc
 * @brief test of the source.extend_to() function
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::source;
using gcxmlib::degree;
using gcxmlib::trail;
using std::chrono::seconds;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-0.3,0.3);

  auto s = std::chrono::system_clock::now();

  const gcxmlib::timestamp_t t0 = gcxmlib::now();
  source p0(1,0,0,t0+seconds(0),degree(2.0));
  source p1(0,1,0,t0+seconds(5),degree(10.0));
  trail arc(p0,p1);

  arc.dump();
  printf("\n\n");
  arc.dump_arc();
  printf("\n\n");
  arc.dump_error();
  printf("\n\n");

  for (size_t i=0; i<24; i++) {
    const double f = 2.0*i/24.0;
    const auto p = p0.extend_to(p1,f);
    p.dump();
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
