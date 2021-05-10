/**
 * @file example_matrix3_chol.cc
 * @brief test of the solve_chol3 function.
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
  p.dump();

  printf("\n");

  printf("# Define `vector3` _q_ with random arguments.\n"
         "# The elements of _q_ should be [%.5lf %.5lf %.5lf].\n", x, y, z);
  q.dump();

  printf("\n");

  {
    printf("# Define the identity matrix.\n");
    const matrix3 I;
    printf("# I = \n");
    I.dump();

    printf("# Solve Ix = p:\n  x = ");
    solve_chol3(I,p).dump();
    printf("\n");

    printf("# Solve Ix = q:\n  x = ");
    solve_chol3(I,q).dump();
    printf("\n");
  }

  {
    printf("# Define a positive definite random matrix _A_.\n");
    std::array<double,9> arr;
    for (size_t i=0; i<9; i++) arr[i] = unif(gen);
    const matrix3 tmp(arr);
    const matrix3 A(tmp*tmp.T());
    printf("# A = \n");
    A.dump();
    printf("\n");

    printf("# Solve Ax = p:\n  x = ");
    const auto v = solve_chol3(A,p);
    v.dump();
    printf("\n");

    printf("# Ax = \n");
    (A*v).dump();
    printf("\n");


    printf("# Solve Ax = q:\n  x = ");
    const auto w = solve_chol3(A,q);
    w.dump();
    printf("\n");

    printf("# Ax = \n");
    (A*w).dump();
    printf("\n");

    printf("# Eigen vector of _A_.\n");
    vector3 a = eigen_pow(A);
    vector3 b = A*a;
    vector3 c = A*b;
    vector3 d = A*c;
    printf("# eig(A) = \n  ");     a.dump();
    printf("# A*eig(A) = \n  ");   b.dump();
    printf("# A*A*eig(A) = \n  "); c.dump();
    printf("# A*A*A*eig(A) = \n  "); d.dump();

    printf("\n");
    printf("# Vector lengths:\n"
           "  %lf %lf %lf %lf\n", a.d, b.d, c.d, d.d);
    printf("# Ratios:\n"
           "  %lf %lf %lf\n", b.d/a.d, c.d/b.d, d.d/c.d);
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
