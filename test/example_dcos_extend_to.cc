/**
 * @file example_docs_extend_to.cc
 * @brief test of the dcos.extend_to() function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::minor_arc;
using gcxmlib::dcos;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# case 1: p0=(1,0,0) and p1=(0,1,0).\n");
    const dcos p0(1,0,0), p1(0,1,0);
    const minor_arc arc(p0,p1);
    arc.dump();
    printf("\n\n");
    for (size_t i=0; i<12; i++) {
       const double f = i/11.;
       const auto p = p0.extend_to(p1,f);
       p.dump();
    }
    printf("\n\n");
    for (size_t i=0; i<12; i++) {
       const double f = 2.5*i/11.-1.0;
       const auto p = p0.extend_to(p1,f);
       p.dump();
    }
  }

   printf("\n\n");

  {
    printf("# case 2: two random points.\n");
    const dcos p0(unif(gen),unif(gen),unif(gen));
    const dcos p1(unif(gen),unif(gen),unif(gen));
    const minor_arc arc(p0,p1);
    arc.dump();
    printf("\n\n");
    for (size_t i=0; i<12; i++) {
      const double f = i/11.;
      const auto p = p0.extend_to(p1,f);
      p.dump();
    }
    printf("\n\n");
    for (size_t i=0; i<12; i++) {
      const double f = 2.5*i/11.-1.0;
      const auto p = p0.extend_to(p1,f);
      p.dump();
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
