/**
 * @file example_footprint.cc
 * @brief functional test of the footprint class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::footprint;
using gcxmlib::longitude;
using gcxmlib::latitude;
using gcxmlib::arcsec;
using gcxmlib::timestamp_t;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-M_PI,M_PI);
  double x(unif(gen)), y(unif(gen)), z(unif(gen));
  const timestamp_t t0 = gcxmlib::now();
  footprint p(1,0,0,t0,arcsec(1.0)), q(x,y,z,t0,arcsec(3.0));
  double d = std::sqrt(x*x+y*y+z*z);

  auto s = std::chrono::system_clock::now();

  printf("# Define `direction_cosine` _p_ without argument.\n"
         "# The elements of _p_ should be\n"
         "#     [%.5lf %.5lf %.5lf].\n", 1.0, 0.0, 0.0);
  p.print();

  printf("\n"
         "# the distance to _p_ should be [%.5lf].\n"
         "# p.d = %.5lf\n", 1.0, p.d);

  printf("\n"
         "# Define `direction_cosine` _q_ with random arguments.\n"
         "# The elements of _q_ should be\n"
         "#     [%.5lf %.5lf %.5lf].\n", x/d, y/d, z/d);
  q.print();

  printf("\n"
         "# the distance to _q_ should be [%.5lf].\n"
         "# q.d = %.5lf\n", 1.0, q.d);

  longitude lon(unif(gen));
  latitude lat(unif(gen)/2.0);
  footprint r(lon,lat,t0,arcsec(5.0));
  printf("\n"
         "# Define `direction_cosine` _r_ with (lon,lat) pair.\n"
         "# The elements of _r_ should be\n"
         "#     [%.5lf %.5lf %.5lf].\n",
         std::cos(lon.radian)*std::cos(lat.radian),
         std::sin(lon.radian)*std::cos(lat.radian),
         std::sin(lat.radian));
  r.print();

  printf("\n"
         "# the distance to _r_ should be [%.5lf].\n"
         "# r.d = %.5lf\n", 1.0, r.d);

  printf("\n"
         "# Calculate an inner product of _q_ and _p_.\n"
         "# The result should be [%.5lf].\n"
         "# q.inner_product(p) = %.5lf\n",
         1.0*x/d, q.inner_product(p));

  printf("\n"
         "# Calculate an inner product of _q_ and [1,1,1].\n"
         "# The result should be [%.5lf].\n"
         "# q.inner_product({1,1,1}) = %.5lf\n",
         (x+y+z)/d, q.inner_product({1,1,1}));

  printf("\n"
         "# Calculate `cos(d)` between _q_ and _p_.\n"
         "# The result should be [%.5lf].\n"
         "# q.separation_cosine(p) = %.5lf\n",
         1.0*x/d, q.separation_cosine(p));

  printf("\n"
         "# Calculate `cos(d)` between _q_ and [1,1,1].\n"
         "# The result should be [%.5lf].\n"
         "# q.separation_cosine(p) = %.5lf\n",
         (x+y+z)/d/std::sqrt(3.0), q.separation_cosine({1,1,1}));

  printf("\n"
         "# Calculate the separation `d` between _q_ and _p_ in radian.\n"
         "# The result should be [%.5lf].\n"
         "# q.separation(p) = %.5lf\n",
         std::acos(1.0*x/d), q.separation(p).radian);

  printf("\n"
         "# Calculate the separation `d` between _q_ and [1,1,1] in radian.\n"
         "# The result should be [%.5lf].\n"
         "# q.separation(p) = %.5lf\n",
         std::acos((x+y+z)/d/std::sqrt(3.0)), q.separation({1,1,1}).radian);

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
