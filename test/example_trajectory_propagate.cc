/**
 * @file example_trajectory_propagate.cc
 * @brief test of the trajectory.propagate function.
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
using msec = std::chrono::milliseconds;
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

  { // trajectory without acceleration.
    const double z0(ang(gen)),z1(ang(gen)),z2(ang(gen));
    const gcxmlib::matrix3 R = Rz(z2)*Ry(z1)*Rz(z0);

    std::vector<trail> tlist;
    const footprint q0 = R*p0;
    const footprint q1 = R*p1;
    const trail trail1(q0,q1);
    printf("# great circle\n");
    trail1.dump();
    printf("\n\n");
    printf("# trail arc\n");
    trail1.dump_arc();
    printf("\n\n");
    tlist.push_back(trail1);

    for (size_t i=0; i<5; i++) {
      const timestamp_t T0 = advance_timestamp(t0,seconds(5+5*i));
      const timestamp_t T1 = advance_timestamp(T0,seconds(1));
      const footprint Q0 = trail1.propagate(T0);
      const footprint Q1 = trail1.propagate(T1);
      const trail traili(Q0,Q1);
      printf("# propagated trail arc %ld\n", i+1);
      traili.dump_arc();
      printf("\n\n");
      tlist.push_back(traili);
    }

    const trajectory trj(tlist);
    trj.dump_arc(32);
    printf("\n\n");

    for (size_t i=0; i<11; i++) {
      const timestamp_t T = advance_timestamp(t0,seconds(5*i));
      const footprint q = trj.propagate(T);
      q.dump();
    }
    printf("\n\n");
  }

  { // trajectory with acceleration.
    const double z0(ang(gen)),z1(ang(gen)),z2(ang(gen));
    const gcxmlib::matrix3 R = Rz(z2)*Ry(z1)*Rz(z0);

    const footprint q0 = R*p0;
    const footprint q1 = R*p1;
    const trail trail1(q0,q1);
    printf("# great circle\n");
    trail1.dump();
    printf("\n\n");
    printf("# trail arc\n");
    trail1.dump_arc();
    printf("\n\n");
    trajectory trj(trail1);

    long dt(0);
    for (size_t i=0; i<5; i++) {
      dt -= 100*i;
      const timestamp_t T0 = advance_timestamp(t0,seconds(5+5*i));
      const timestamp_t T1 = advance_timestamp(T0,seconds(1));
      const footprint Q0 = trail1.propagate(T0);
      const footprint Q1 = trail1.propagate(T1);
      const footprint R0 = footprint(Q0.l,Q0.m,Q0.n,Q0.t+msec(dt),Q0.s);
      const footprint R1 = footprint(Q1.l,Q1.m,Q1.n,Q1.t+msec(dt),Q1.s);
      const trail traili(R0,R1);
      printf("# propagated trail arc %ld\n", i+1);
      trj.append(traili);
      traili.dump_arc();
      printf("\n\n");
    }

    trj.dump_arc(32);
    printf("\n\n");

    { // propagate to T with acceleration
      long dt(0);
      for (size_t i=0; i<11; i++) {
        const timestamp_t T = advance_timestamp(t0,seconds(5*i)+msec(dt));
        const footprint q = trj.propagate(T);
        q.dump();
        dt -= 100*i;
      }
    }
    printf("\n\n");

    { // propagatee to T without acceleration
      long dt(0);
      for (size_t i=0; i<11; i++) {
        const timestamp_t T = advance_timestamp(t0,seconds(5*i)+msec(dt));
        const footprint r = trail1.propagate(T);
        r.dump();
        dt -= 100*i;
      }
    }
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
