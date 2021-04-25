/**
 * @file spchord.h
 * @brief header file for the spchord library
 * @author Ryou Ohsawa
 * @year 2021
 */
#ifndef __SPCHORD_H_INCLUDE
#define __SPCHORD_H_INCLUDE

#include <cstdio>
#include <cmath>
#include <chrono>
#include <utility>
#include <vector>
#include <set>


namespace spchord {
  constexpr bool __debug__ = false;
  constexpr double radian_to_arcsec = 3600./180./M_PI;
  constexpr double arcsec_to_radian = M_PI/180./3600.;

  typedef std::chrono::duration<double> sec_t;
  typedef std::chrono::time_point
    <std::chrono::high_resolution_clock, sec_t> timestamp_t;

  /** a vertex in a three-dimensional space. */
  class vector3 {
  public:
    /**
     * @brief construct a `vector3` instance initialized with (1,0,0).
     */
    vector3() : vector3(1.0,0.0,0.0) {}

    /**
     * @brief construct a `vector3` instance with (x,y,z).
     * @param[in] _x: coordinate x.
     * @param[in] _y: coordinate y.
     * @param[in] _z: coordinate z.
     */
    vector3(const double _x, const double _y, const double _z)
      : x(_x), y(_y), z(_z), d(std::sqrt(x*x+y*y+z*z))
    {}

    /**
     * @brief return an inner product with a `vector3` instance `p`.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    inner_product(const vector3 p) const
    { return p.x*x + p.y*y + p.z*z; }


    /**
     * @brief return `cos(d)` where `d` is the angular separation.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    separation_cosine(const vector3 p) const
    { return inner_product(p)/p.d/d; }

    /**
     * @brief return the separation angle from `p` in radian.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    separation(const vector3 p) const
    { return std::acos(separation_cosine(p)); }

    /**
     * @brief return the separation angle from `p` in arcsecond.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    separation_arcsec(const vector3 p) const
    { return separation(p)*radian_to_arcsec; }

    /**
     * @brief dump all the elements to stdout.
     */
    void dump() const
    { printf("%.5lf %.5lf %.5lf\n", x, y, z); }

    const double x; /** x-coordinate. */
    const double y; /** y-coordinate. */
    const double z; /** z-coordinate. */
    const double d; /** distance from the origin. */
  };


  class direction_cosine : public vector3 {
  public:
    /**
     * @brief construct a `direction_cosine` instance initialized with (1,0,0).
     */
    direction_cosine() : direction_cosine(1.0, 0.0, 0.0) {}

    /**
     * @brief construct a `direction_cosine` instance with (x,y,z).
     * @param[in] _x: x-coordinate.
     * @param[in] _y: y-coordinate.
     * @param[in] _z: z-coordinate.
     */
    direction_cosine(const double _x, const double _y, const double _z)
      : vector3(nx(_x,_y,_z),ny(_x,_y,_z),nz(_x,_y,_z)), l(x), m(y), n(z)
    {}

    /**
     * @brief construct a `direction_cosine` instance with `vector3`.
     * @parma[in] p: an instance of `vector3` class.
     */
    direction_cosine(const vector3 p)
      : direction_cosine(p.x, p.y, p.z)
    {}

    const double& l; /** l-element of direction cosine (reference to x) */
    const double& m; /** m-element of direction cosine (reference to y) */
    const double& n; /** n-element of direction cosine (reference to z) */

  private:
    /** @brief normalize x-coordinate in initialization. */
    const double
    nx(const double _x, const double _y, const double _z) const
    { return _x/std::sqrt(_x*_x+_y*_y+_z*_z); }
    /** @brief normalize y-coordinate in initialization. */
    const double
    ny(const double _x, const double _y, const double _z) const
    { return _y/std::sqrt(_x*_x+_y*_y+_z*_z); }
    /** @brief normalize z-coordinate in initialization. */
    const double
    nz(const double _x, const double _y, const double _z) const
    { return _z/std::sqrt(_x*_x+_y*_y+_z*_z); }
  };
  /** define _dcos_ as a shorthand of direction_cosine. */
  typedef direction_cosine dcos;


  class source : public direction_cosine {
  public:
    /**
     * @brief construct a `source` instance
     */
    source() : source(1.0, 0.0, 0.0) {}

    /**
     * @brief
     */
    source(const double _x, const double _y, const double _z)
      : direction_cosine(_x, _y, _z), t(now()), s(1.0) {}


    const timestamp_t t; /** timestamp of the measurement. */
    const double s;      /** uncertainty of the position in arcsecond. */
  private:
    const timestamp_t now() const
    { return std::chrono::high_resolution_clock::now(); }
  };
}

#endif  // __SPCHORD_H_INCLUDE
