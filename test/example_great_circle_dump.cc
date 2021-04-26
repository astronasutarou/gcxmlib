/**
 * @file example_great_circle_dump.cc
 * @brief test of the great_circle.dump() function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::great_circle;
using gcxmlib::dcos;
using gcxmlib::longitude;
using gcxmlib::latitude;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);

  auto s = std::chrono::system_clock::now();

  {
    printf("# create `great_circle` with (l,m,n)-components.\n");
    double l(unif(gen)),m(unif(gen)),n(unif(gen));
    great_circle gc(l,m,n);
    gc.pole.dump();
    printf("\n\n");
    gc.dump();
  }

  printf("\n\n");

  {
    printf("# create `great_circle` with a `direction_cosine`.\n");
    dcos pole(unif(gen),unif(gen),unif(gen));
    great_circle gc(pole);
    gc.pole.dump();
    printf("\n\n");
    gc.dump();
  }

  printf("\n\n");

  {
    printf("# create `great_circle` with (lon,lat).\n");
    longitude lon(unif(gen));
    latitude lat(unif(gen));
    printf("%lf %lf\n", (double)lon, (double)lat);
    great_circle gc(lon,lat);
    printf("%lf\n", (double)gc.pole.lon);
    gc.pole.dump();
    printf("\n\n");
    gc.dump();
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
