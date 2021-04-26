/**
 * @file example_vector3.cc
 * @brief functional test of the vector3 class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::vector3;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);
  double x(unif(gen)), y(unif(gen)), z(unif(gen));
  vector3 p, q(x,y,z);

  auto s = std::chrono::system_clock::now();

  printf("# Define `vector3` _p_ without argument.\n"
         "# The elements of _p_ should be [%.5lf %.5lf %.5lf].\n",
         1.0, 0.0, 0.0);
  p.dump();

  printf("\n"
         "# the distance to _p_ should be [%.5lf].\n"
         "# p.d = %.5lf\n", 1.0, p.d);

  printf("\n"
         "# Define `vector3` _q_ with random arguments.\n"
         "# The elements of _q_ should be [%.5lf %.5lf %.5lf].\n", x, y, z);
  q.dump();

  printf("\n"
         "# the distance to _q_ should be [%.5lf].\n"
         "# q.d = %.5lf\n", std::sqrt(x*x+y*y+z*z), q.d);

  printf("\n"
         "# Calculate an inner product of _q_ and _p_.\n"
         "# The result should be [%.5lf].\n"
         "# q.inner_product(p) = %.5lf\n",
         1.0*x, q.inner_product(p));

  printf("\n"
         "# Calculate an inner product of _q_ and [1,1,1].\n"
         "# The result should be [%.5lf].\n"
         "# q.inner_product({1,1,1}) = %.5lf\n",
         x+y+z, q.inner_product({1,1,1}));

  printf("\n"
         "# Calculate `cos(d)` between _q_ and _p_.\n"
         "# The result should be [%.5lf].\n"
         "# q.separation_cosine(p) = %.5lf\n",
         1.0*x/q.d, q.separation_cosine(p));

  printf("\n"
         "# Calculate `cos(d)` between _q_ and [1,1,1].\n"
         "# The result should be [%.5lf].\n"
         "# q.separation_cosine(p) = %.5lf\n",
         (x+y+z)/q.d/std::sqrt(3.0), q.separation_cosine({1,1,1}));

  printf("\n"
         "# Calculate the separation `d` between _q_ and _p_ in radian.\n"
         "# The result should be [%.5lf].\n"
         "# q.separation(p) = %.5lf\n",
         std::acos(1.0*x/q.d), q.separation(p).radian);

  printf("\n"
         "# Calculate the separation `d` between _q_ and [1,1,1] in radian.\n"
         "# The result should be [%.5lf].\n"
         "# q.separation(p) = %.5lf\n",
         std::acos((x+y+z)/q.d/std::sqrt(3.0)), q.separation({1,1,1}).radian);

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
