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

  auto s = std::chrono::system_clock::now();

  std::array<double,9> arr1, arr2;
  for (size_t i=0; i<9; i++) {
    arr1[i] = unif(gen);
    arr2[i] = unif(gen);
  }
  const matrix3 A(arr1), B(arr2);

  printf("# Define two random matrix _A_ and _B_.\n");
  printf("\n");

  printf("A = \n");
  A.dump();
  printf("\n");

  printf("B = \n");
  B.dump();
  printf("\n");

  printf("(A+1) = \n");
  (A+1.).dump();
  printf("\n");
  printf("(3+A) = \n");
  (3.+A).dump();
  printf("\n");

  printf("(A-1) = \n");
  (A-1.).dump();
  printf("\n");
  printf("(3-A) = \n");
  (3.-A).dump();
  printf("\n");

  printf("(A*1) = \n");
  (A*2.).dump();
  printf("\n");
  printf("(3*A) = \n");
  (3.*A).dump();
  printf("\n");

  printf("(A/3) = \n");
  (A/3.).dump();
  printf("\n");

  printf("-A = \n");
  (-A).dump();
  printf("\n");

  printf("(A+B) = \n");
  (A+B).dump();
  printf("\n");

  printf("(A-B) = \n");
  (A-B).dump();
  printf("\n");

  printf("(A*B) = \n");
  (A*B).dump();
  printf("\n");

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
