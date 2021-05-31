/**
 * @file example_vector3_outer_product.cc
 * @brief test of the vector3.outer_product() function.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::vector3;
using gcxmlib::dcos;
using gcxmlib::footprint;


int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-2,2);

  auto s = std::chrono::system_clock::now();

  {
    printf("# outer_product() for `vector3` class.\n");
    vector3 p(unif(gen), unif(gen), unif(gen));
    vector3 q(unif(gen), unif(gen), unif(gen));
    p.print();
    q.print();

    auto r = p.outer_product(q);
    r.print();
  }

  printf("\n\n");

  {
    printf("# outer_product() for `direction_cosine` class.\n");
    dcos p(unif(gen), unif(gen), unif(gen));
    dcos q(unif(gen), unif(gen), unif(gen));
    p.print();
    q.print();

    auto r = p.outer_product(q);
    r.print();
    auto n = p.get_pole(q);
    n.print();
  }

  printf("\n\n");

  {
    printf("# outer_product() for `footprint` class.\n");
    footprint p(unif(gen), unif(gen), unif(gen));
    footprint q(unif(gen), unif(gen), unif(gen));
    p.print();
    q.print();

    auto r = p.outer_product(q);
    r.print();
    auto n = p.get_pole(q);
    n.print();
  }

  printf("\n\n");

  {
    printf("# outer_product() between `direction_cosine` and `vector3`.\n");
    dcos p(unif(gen), unif(gen), unif(gen));
    vector3 q(unif(gen), unif(gen), unif(gen));
    p.print();
    q.print();

    auto r = p.outer_product(q);
    r.print();
    auto n = p.get_pole(q);
    n.print();
  }

  printf("\n\n");

  {
    printf("# outer_product() between `footprint` and `vector3`.\n");
    footprint p(unif(gen), unif(gen), unif(gen));
    vector3 q(unif(gen), unif(gen), unif(gen));
    p.print();
    q.print();

    auto r = p.outer_product(q);
    r.print();
    auto n = p.get_pole(q);
    n.print();
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
