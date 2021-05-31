/**
 * @file example_trajectory.cc
 * @brief functional test of the trajectory class.
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>
#include <unistd.h>

using gcxmlib::trajectory;
using gcxmlib::trail;
using gcxmlib::dcos;
using gcxmlib::footprint;
using gcxmlib::degree;
using gcxmlib::timestamp_t;
using gcxmlib::advance_timestamp;
using std::chrono::seconds;
using namespace gcxmlib::matrix;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_real_distribution<double> ang(0,2*M_PI);

  auto s = std::chrono::system_clock::now();

  const timestamp_t t0 = gcxmlib::now();
  const timestamp_t t1 = advance_timestamp(t0, seconds(1));
  const footprint p0(1,0,0,t0,degree(0.1));
  const dcos tmp = Rz(degree(5.0))*p0;
  const footprint p1(tmp.l,tmp.m,tmp.n,t1,degree(0.1));
  const trail trail0(p0,p1);
  printf("# orignal great circle\n");
  trail0.print();
  printf("\n\n");
  printf("# orignal trail arc\n");
  trail0.print_arc();
  printf("\n\n");

  { // trajectory from trail list.
    const double z0(ang(gen)),z1(ang(gen)),z2(ang(gen));
    const gcxmlib::matrix3 R = Rz(z2)*Ry(z1)*Rz(z0);

    std::vector<trail> tlist;
    const footprint q0 = R*p0;
    const footprint q1 = R*p1;
    const trail trail1(q0,q1);
    printf("# great circle\n");
    trail1.print();
    printf("\n\n");
    printf("# trail arc\n");
    trail1.print_arc();
    printf("\n\n");
    tlist.push_back(trail1);

    for (size_t i=0; i<5; i++) {
      const timestamp_t T0 = advance_timestamp(t0,seconds(5+5*i));
      const timestamp_t T1 = advance_timestamp(T0,seconds(1));
      const footprint Q0 = trail1.propagate(T0);
      const footprint Q1 = trail1.propagate(T1);
      const trail traili(Q0,Q1);
      printf("# propagated trail arc %ld\n", i+1);
      traili.print_arc();
      printf("\n\n");
      tlist.push_back(traili);
    }

    const trajectory trj(tlist);
    trj.print_arc(32);
    printf("\n\n");
  }

  { // trajectory by sequentially appending trails.
    const double z0(ang(gen)),z1(ang(gen)),z2(ang(gen));
    const gcxmlib::matrix3 R = Rz(z2)*Ry(z1)*Rz(z0);

    const footprint q0 = R*p0;
    const footprint q1 = R*p1;
    const trail trail1(q0,q1);
    printf("# great circle\n");
    trail1.print();
    printf("\n\n");
    printf("# trail arc\n");
    trail1.print_arc();
    printf("\n\n");
    trajectory trj(trail1);

    for (size_t i=0; i<5; i++) {
      const timestamp_t T0 = advance_timestamp(t0,seconds(5+5*i));
      const timestamp_t T1 = advance_timestamp(T0,seconds(1));
      const footprint Q0 = trail1.propagate(T0);
      const footprint Q1 = trail1.propagate(T1);
      const trail traili(Q0,Q1);
      printf("# propagated trail arc %ld\n", i+1);
      traili.print_arc();
      printf("\n\n");
      trj.append(traili);
    }

    trj.print_arc(32);
    printf("\n\n");
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
