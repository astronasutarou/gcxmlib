/**
 * @file example_great_circle_separation.cc
 * @brief test of the great_circle.separation() function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::great_circle;
using gcxmlib::dcos;
using gcxmlib::longitude;
using gcxmlib::latitude;
using gcxmlib::degree;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> pos(-1,1);
  std::uniform_real_distribution<double> ang(0,90);

  auto s = std::chrono::system_clock::now();

  printf("# create `great_circle` with the pole of (0,0,1)\n");
  great_circle p(0,0,1);

  for (size_t i=0; i<5; i++) {
    const longitude lon(degree(0.0));
    const latitude lat(degree(ang(gen)));
    printf("# create `great_circle` _q_ with (%lf deg,%lf deg).\n",
           lon.degree, lat.degree);
    const great_circle q(lon,lat);
    printf("# separation angle between _p_ and _q_ should be %lf.\n",
           90.0-lat.degree);
    printf("p.separation(q).degree = %lf\n", p.separation(q).degree);
  }

  printf("\n");

  for (size_t i=0; i<5; i++) {
    const double x(pos(gen)), y(pos(gen)), z(pos(gen));
    const dcos q(x,y,z);
    const double d = std::abs(90.0-dcos{0,0,1}.separation(q).degree);
    printf("# create `direction_cosine` _q_ with (%lf,%lf,%lf).\n",x,y,z);
    printf("# separation angle between _p_ and _q_ should be %lf.\n", d);
    printf("p.separation(q).degree = %lf\n", p.separation(q).degree);
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
