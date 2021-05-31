/**
 * @file example_matrix3.cc
 * @brief functional test of the matrix3 class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::matrix3;
using gcxmlib::vector3;
using gcxmlib::dcos;
using gcxmlib::footprint;
using gcxmlib::degree;
using namespace gcxmlib::matrix;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> unif(-1,1);
  double x(unif(gen)), y(unif(gen)), z(unif(gen));
  vector3 p(1,0,0), q(x,y,z);
  footprint r(x,y,z);

  auto s = std::chrono::system_clock::now();

  printf("# Define `vector3` _p_ without argument.\n"
         "# The elements of _p_ should be [%.5lf %.5lf %.5lf].\n",
         1.0, 0.0, 0.0);
  p.print();

  printf("\n");

  printf("# Define `vector3` _q_ with random arguments.\n"
         "# The elements of _q_ should be [%.5lf %.5lf %.5lf].\n", x, y, z);
  q.print();

  printf("\n");
  {
    printf("# Define `matrix3` _M_ without argument.\n"
           "# This returns the identity matrix.\n"
           "M = \n");
    matrix3 M;
    M.print();
    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*p).print() = \n  ");
    (M*p).print();

    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*q).print() = \n  ");
    (M*q).print();
  }

  printf("\n");
  {
    printf("# Define `matrix3` _M_ as a rotation matrix around the x-axis.\n"
           "\nRx(90) = \n");
    matrix3 M = Rx(degree(90));
    M.print();
    printf("\n");

    printf("# Transpose of `matrix3`\n"
           "\nRx(90).T = \n");
    M.T().print();
    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*p).print() = \n  ");
    (M*p).print();
  }

  printf("\n");
  {
    printf("# Define `matrix3` _M_ as a rotation matrix around the y-axis.\n"
           "\nRy(90) = \n");
    matrix3 M = Ry(degree(90));
    M.print();
    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*p).print() = \n  ");
    (M*p).print();

    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*q).print() = \n  ");
    (M*q).print();
  }

  printf("\n");
  {
    printf("# Define `matrix3` _M_ as a rotation matrix around the z-axis.\n"
           "\nRz(90) = \n");
    matrix3 M = Rz(degree(90));
    M.print();
    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*p).print() = \n  ");
    (M*p).print();
    printf("\n");

    printf("# `vector3` can be multiplied by `matrix3` from left.\n"
           "(M*q).print() = \n  ");
    (M*q).print();
  }

  printf("\n");
  {
    printf("# Two `matrix3` instances can be multiplied.\n");
    matrix3 Mx = Rx(degree(90));
    matrix3 Mz = Rz(degree(90));
    printf("\nRx(90) = \n");
    Mx.print();
    printf("\n");

    printf("\nRz(90) = \n");
    Mz.print();
    printf("\n");

    printf("(Mx*Mz).print() = \n");
    (Mx*Mz).print();
    printf("\n");

    printf("(Mz*Mx).print() = \n");
    (Mz*Mx).print();
  }

  printf("\n");

  {
    printf("# _r_ is a `footprint` instance.\n");
    r.print();
    printf("\n");

    printf("# the footprint instance can be rotated by `matrix3`.\n");
    const matrix3 Mz = Rz(degree(15));

    printf("\nRz(15) = \n");
    Mz.print();
    printf("\n");

    printf("(Mz*r).print() = \n  ");
    (Mz*r).print();
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
