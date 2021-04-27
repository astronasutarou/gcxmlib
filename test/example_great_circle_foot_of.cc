/**
 * @file example_great_circle_foot_of.cc
 * @brief test of the great_circle.foot_of() function.
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


  {
    printf("# create `great_circle` with the pole of (0,0,1)\n");
    great_circle gc(0,0,1);
    gc.dump();
    printf("\n\n");
    for (size_t i=0; i<50; i++) {
      const double x(pos(gen)), y(pos(gen)), z(pos(gen));
      const dcos p(x,y,z);
      const dcos f = gc.foot_of(p);
      printf("%lf %lf %lf %lf %lf %lf\n",p.l,p.m,p.n,f.l-p.l,f.m-p.m,f.n-p.n);
    }
  }

  printf("\n\n");

  {
    const longitude lon(degree(4.0*ang(gen)));
    const latitude lat(degree(ang(gen)));
    printf("# create `great_circle` with the pole of (%lf deg,%lf deg)\n",
           lon.degree, lat.degree);
    great_circle gc(lon,lat);
    gc.dump();
    printf("\n\n");
    for (size_t i=0; i<50; i++) {
      const double x(pos(gen)), y(pos(gen)), z(pos(gen));
      const dcos p(x,y,z);
      const dcos f = gc.foot_of(p);
      printf("%lf %lf %lf %lf %lf %lf\n",p.l,p.m,p.n,f.l-p.l,f.m-p.m,f.n-p.n);
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
