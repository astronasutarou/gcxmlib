/**
 * @file gcxmlib.h
 * @brief header file for the gcxmlib library
 * @author Ryou Ohsawa
 * @year 2021
 */
#ifndef __GCXMLIB_H_INCLUDE
#define __GCXMLIB_H_INCLUDE
#define _USE_MATH_DEFINES

#include <cstdio>
#include <cmath>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <stdexcept>


namespace gcxmlib {
  using namespace std::chrono_literals;

  constexpr bool __debug__ = false;
  constexpr double __epsilon__ = 1e-15;
  constexpr double __exact_zero__ = 0.0;
  constexpr double radian_to_degree = 180./M_PI;
  constexpr double degree_to_radian = M_PI/180.;
  constexpr double radian_to_arcmin = 60.*180./M_PI;
  constexpr double arcmin_to_radian = M_PI/180./60.;
  constexpr double radian_to_arcsec = 3600.*180./M_PI;
  constexpr double arcsec_to_radian = M_PI/180./3600.;

  typedef std::chrono::duration<double> sec_t;
  typedef std::chrono::time_point
      <std::chrono::system_clock> timestamp_t;

  enum class angle_range
  {
    zero_to_twopi,       /** range [0, 2pi)      (w/wrap)   */
    minus_pi_to_pi,      /** range [-pi, pi)     (w/wrap)   */
    zero_to_pi,          /** range [0, pi]       (w/o wrap) */
    minus_pi_2_to_pi_2   /** range [-pi/2, pi/2] (w/o wrap) */
  };

  template<angle_range _range>
  class base_angle {
  public:
    /** disable the constructor without an argument. */
    base_angle() = delete;

    /**
     * @brief construct an `base_angle` instance.
     * @param _r: angle value in radian.
     *
     * The behavior of this constructor change with `angle_range`.
     * In case that the `angle_range` is `zero_to_twopi` or `minus_pi_to_pi`,
     * the angle is automatically wrapped in the domain. Instead, incase that
     * the `angle_range` is `zero_to_pi` or `minus_pi_2_to_pi_2`, the
     * constructor will throw `range_error` if the angle violates the domain.
     */
    base_angle(double _r)
      : radian(wrap(_r)),degree(to_degree()),
        arcmin(to_arcmin()),arcsec(to_arcsec()) {}

    /**
     * @brief construct an `base_angle` instance from another `base_angle`.
     * @param ang: a `base_angle` instance.
     */
    template<angle_range __range>
    base_angle(const base_angle<__range>& ang)
      : base_angle(ang.radian) {}

    const double radian; /** angle in radian */
    const double degree; /** angle in degree */
    const double arcmin; /** angle in arcmin */
    const double arcsec; /** angle in arcsec */

    /**
     * @brief implicit conversion to `double` returns `radian`.
     */
    operator double() const { return radian; }

    /**
     * @brief `unary minus` operator.
     */
    const base_angle<_range>
    operator-() const
    { return base_angle<_range>(-radian); }

    /**
     * @brief `add` operation with a floating point value.
     * @param ang: a `base_angle` intance.
     * @param val: a floating point value.
     */
    friend const base_angle<_range>
    operator+(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian+val); }
    /**
     * @brief `add` operation with a floating point value.
     * @param val: a floating point value.
     * @param ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator+(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val+ang.radian); }
    /**
     * @brief `subtract` operation with a floating point value.
     * @param ang: a `base_angle` intance.
     * @param val: a floating point value.
     */
    friend const base_angle<_range>
    operator-(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian-val); }
    /**
     * @brief `subtract` operation with a floating point value.
     * @param val: a floating point value.
     * @param ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator-(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val-ang.radian); }
    /**
     * @brief `multiply` operation with a floating point value.
     * @param ang: a `base_angle` intance.
     * @param val: a floating point value.
     */
    friend const base_angle<_range>
    operator*(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian*val); }
    /**
     * @brief `multiply` operation with a floating point value.
     * @param val: a floating point value.
     * @param ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator*(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(ang.radian*val); }
    /**
     * @brief `divide` operation with a floating point value.
     * @param ang: a `base_angle` intance.
     * @param val: a floating point value.
     */
    friend const base_angle<_range>
    operator/(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian/val); }
    /**
     * @brief `divide` operation with a floating point value.
     * @param val: a floating point value.
     * @param ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator/(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val/ang.radian); }

    /**
     * @brief `add` operation with another `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const base_angle<_range>
    operator+(const base_angle<__range>& ang) const
    { return base_angle<_range>(radian+ang.radian); }
    /**
     * @brief `subtract` operation with another `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const base_angle<_range>
    operator-(const base_angle<__range>& ang) const
    { return base_angle<_range>(radian-ang.radian); }

    /**
     * @brief equality operator to a `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator==(const base_angle<__range>& ang) const
    { return (radian == ang.radian); }
    /**
     * @brief inequality operator to a `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator!=(const base_angle<__range>& ang) const
    { return (radian != ang.radian); }
    /**
     * @brief lesser operator to a `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator<(const base_angle<__range>& ang) const
    { return (radian < ang.radian); }
    /**
     * @brief lesser-or-equal operator to a `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator<=(const base_angle<__range>& ang) const
    { return (radian <= ang.radian); }
    /**
     * @brief greater operator to a `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator>(const base_angle<__range>& ang) const
    { return (radian > ang.radian); }
    /**
     * @brief greater-or-equal operator to a `base_angle` instance.
     * @param ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator>=(const base_angle<__range>& ang) const
    { return (radian >= ang.radian); }

  private:
    /**
     * @brief wrap the angle within the domain of `base_angle`.
     * @param ang: angle value in radian.
     */
    const double
    wrap(double arg) const
    {
      switch (_range) {
      case angle_range::zero_to_twopi:
        if (arg < 0)
          arg += (2*M_PI)*(1-std::floor(arg/(2*M_PI)));
        if (arg >= 2*M_PI)
          arg -= (2*M_PI)*(std::floor(arg/(2*M_PI)));
        return arg;
        break;
      case angle_range::minus_pi_to_pi:
        arg += M_PI;
        if (arg < 0)
          arg += (2*M_PI)*(1-std::floor(arg/(2*M_PI)));
        if (arg >= 2*M_PI)
          arg -= (2*M_PI)*(std::floor(arg/(2*M_PI)));
        return arg-M_PI;
        break;
      case angle_range::zero_to_pi:
        if (arg < 0 || arg > M_PI)
          throw std::range_error("value exceeds the range [0,pi].");
        return arg;
        break;
      case angle_range::minus_pi_2_to_pi_2:
        if (arg < -M_PI_2 || arg > M_PI_2)
          throw std::range_error("value exceeds the range [-pi/2,pi/2].");
        return arg;
        break;
      default:
        throw std::invalid_argument("invalid range specified.");
      }
    }
    /** convert radian to degree. */
    const double
    to_degree() const { return radian*radian_to_degree; }
    /** convert radian to arcmin. */
    const double
    to_arcmin() const { return radian*radian_to_arcmin; }
    /** convert radian to arcsec. */
    const double
    to_arcsec() const { return radian*radian_to_arcsec; }
  };

  /** general purpose angle class. */
  typedef base_angle<angle_range::zero_to_twopi> angle;
  /** `longitude` is an alias to `angle`. */
  typedef base_angle<angle_range::zero_to_twopi> longitude;
  /** `latitude` is defined within [-pi/2,pi/2]. */
  typedef base_angle<angle_range::minus_pi_2_to_pi_2> latitude;

  /** a helper function to make `angle` in radian. */
  const angle radian(const double ang)
  { return angle(ang); }
  /** a helper function to make `angle` in degree. */
  const angle degree(const double ang)
  { return angle(ang*degree_to_radian); }
  /** a helper function to make `angle` in arcmin. */
  const angle arcmin(const double ang)
  { return angle(ang*arcmin_to_radian); }
  /** a helper function to make `angle` in arcsec. */
  const angle arcsec(const double ang)
  { return angle(ang*arcsec_to_radian); }

  namespace literals {
    const angle operator"" rad(long double ang)
    { return radian(ang); }
    const angle operator"" deg(long double ang)
    { return degree(ang); }
    const angle operator"" amin(long double ang)
    { return arcmin(ang); }
    const angle operator"" asec(long double ang)
    { return arcsec(ang); }
  }

  class vector3 {
  public:
    /** disable the constructor without arguments. */
    vector3() = delete;

    /**
     * @brief construct a `vector3` instance with (x,y,z).
     * @param _x: coordinate x.
     * @param _y: coordinate y.
     * @param _z: coordinate z.
     */
    vector3(const double _x, const double _y, const double _z)
      : x(_x), y(_y), z(_z), d(std::sqrt(x*x+y*y+z*z))
    {}

    /**
     * @brief return an inner product with a `vector3` instance `p`.
     * @param p: an instance of `vector3` class.
     */
    const double
    inner_product(const vector3& p) const
    { return p.x*x + p.y*y + p.z*z; }


    /**
     * @brief return `cos(d)` where `d` is the angular separation.
     * @param p: an instance of `vector3` class.
     */
    const double
    separation_cosine(const vector3& p) const
    { return inner_product(p)/p.d/d; }

    /**
     * @brief return the separation angle from `p` in radian.
     * @param p: an instance of `vector3` class.
     */
    const angle
    separation(const vector3& p) const
    { return std::acos(separation_cosine(p)); }

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
    /** disable the constructor without arguments. */
    direction_cosine() = delete;

    /** copy constructor */
    direction_cosine(const direction_cosine& copy)
      : direction_cosine(copy.x,copy.y,copy.z) {}

    /**
     * @brief construct a `direction_cosine` instance with (x,y,z).
     * @param _l: x-coordinate.
     * @param _m: y-coordinate.
     * @param _n: z-coordinate.
     */
    direction_cosine(const double _l, const double _m, const double _n)
      : vector3(nx(_l,_m,_n),ny(_l,_m,_n),nz(_l,_m,_n)), l(x), m(y), n(z),
        lon(calc_longitude()), lat(calc_latitude())
    {}

    /**
     * @brief construct a `direction_cosine` instance with `vector3`.
     * @parma p: an instance of `vector3` class.
     */
    direction_cosine(const vector3& p)
      : direction_cosine(p.x, p.y, p.z)
    {}

    /**
     * @brief construct a `direction_cosine` instance with (lon,lat).
     * @param lon: a `longitude` instance.
     * @param lat: a `latitude` instance.
     */
    direction_cosine(const longitude& _lon, const latitude& _lat)
      : direction_cosine(vx(_lon,_lat),vy(_lon,_lat),vz(_lon,_lat))
    {}

    const double& l; /** l-element of direction cosine (reference to x) */
    const double& m; /** m-element of direction cosine (reference to y) */
    const double& n; /** n-element of direction cosine (reference to z) */
    const longitude lon;
    const latitude lat;

  private:
    /** normalize x-coordinate in initialization. */
    const double
    nx(const double _x, const double _y, const double _z) const
    { return _x/std::sqrt(_x*_x+_y*_y+_z*_z); }
    /** normalize y-coordinate in initialization. */
    const double
    ny(const double _x, const double _y, const double _z) const
    { return _y/std::sqrt(_x*_x+_y*_y+_z*_z); }
    /** normalize z-coordinate in initialization. */
    const double
    nz(const double _x, const double _y, const double _z) const
    { return _z/std::sqrt(_x*_x+_y*_y+_z*_z); }
    /** calculate x-coordinate from (lon,lat) */
    const double
    vx(const longitude& _lon, const latitude& _lat)
    { return std::cos(_lon.radian)*std::cos(_lat.radian); }
    /** calculate y-coordinate from (lon,lat) */
    const double
    vy(const longitude& _lon, const latitude& _lat)
    { return std::sin(_lon.radian)*std::cos(_lat.radian); }
    /** calculate z-coordinate from (lon,lat) */
    const double
    vz(const longitude& _lon, const latitude& _lat)
    { return std::sin(_lat.radian); }
    const longitude
    calc_longitude()
    { return longitude(std::atan2(m,l));}
    const latitude
    calc_latitude()
    { return latitude(std::asin(n)); }
  };
  /** define _dcos_ as a shorthand of direction_cosine. */
  typedef direction_cosine dcos;


  class source : public direction_cosine {
  public:
    /** disable the constructor without arguments. */
    source() = delete;

    /**
     * @brief construct a `source` instance with (x,y,z)
     * @note set `t` and `s` the system time and 1 arcsec, respectivly.
     */
    source(const double _x, const double _y, const double _z)
      : direction_cosine(_x, _y, _z), t(now()), s(arcsec(1.0)) {}

    /**
     * @brief construct a `source` instance with (x,y,z,t)
     * @note set `s` 1 arcsec if not assigned.
     */
    source(const double _x, const double _y, const double _z,
           const timestamp_t _t, const angle& _s = arcsec(1.0))
      : direction_cosine(_x, _y, _z), t(_t), s(_s) {}

    /**
     * @brief construct a `source` instance with (lon,lat,t)
     * @note set `s` 1 arcsec if not assigned.
     */
    source(const longitude& _lon, const latitude& _lat,
           const timestamp_t _t, const angle& _s = arcsec(1.0))
      : direction_cosine(_lon, _lat), t(_t), s(_s) {}

    /**
     * @brief return `true` if another point is inside the range.
     * @param p: another positional instance.
     * @param range: an `angle` instance.
     */
    const bool
    neighbor_to(const vector3& p, const angle& range) const
    {
      const double cosine = std::cos(range.radian);
      const double sepcos = separation_cosine(p);
      return sepcos >= cosine;
    }

    /**
     * @brief return `true` if another point is inside the uncertainty.
     * @param p: another positional instance.
     */
    const bool
    neighbor_to(const vector3& p) const
    { return neighbor_to(p, s.radian); }

    /**
     * @brief return `true` if another point is inside the uncertainty.
     * @param p: another positional instance.
     * @note `range` is set the summation of the both uncertainties
     *       if `p` is an instance of `source`. as `range`.
     */
    const bool
    neighbor_to(const source& p) const
    { return neighbor_to(p, s.radian+p.s.radian); }

    /**
     * @brief return `true` if another point is inside the range.
     * @param p: another positional instance.
     * @param range: an `angle` instance.
     * @param trange: a time duration.
     */
    const bool
    match(const source& p, const angle& range, const sec_t& trange) const
    {
      const double cosine = std::cos(range.radian);
      const double tsep = std::abs(static_cast<sec_t>(t-p.t).count());
      const double sepcos = separation_cosine(p);
      return (sepcos >= cosine) && (tsep < trange.count());
    }

    /**
     * @brief dump all the elements to stdout.
     */
    void dump() const
    {
      const std::time_t tm = std::chrono::system_clock::to_time_t(t);
      const auto dur = t.time_since_epoch();
      const auto sub =
        std::chrono::duration_cast<std::chrono::microseconds>(dur)
        % std::chrono::seconds{1};
      std::stringstream ss;
      ss << std::put_time(std::gmtime(&tm), "%FT%T") << "."
         << std::setfill('0') << std::setw(6) << sub.count();
      printf("%.5lf %.5lf %.5lf %.5lf %s\n",
             x, y, z, s.arcsec, ss.str().c_str());
    }

    const timestamp_t t; /** timestamp of the measurement. */
    const angle s;       /** uncertainty of the position. */

    friend bool
    operator==(const source& lhs, const source& rhs)
    { return true; }
  private:
    const timestamp_t now() const
    { return std::chrono::system_clock::now(); }
  };


  /**
   * @brief calculate the outer product of two positional classes.
   * @param P: the first argument of the outer product.
   * @param Q: the second argument of the outer product.
   */
  const vector3
  outer_product(const vector3& p1, const vector3& p2)
  {
    const double&& x = p1.y*p2.z - p1.z*p2.y;
    const double&& y = p1.z*p2.x - p1.x*p2.z;
    const double&& z = p1.x*p2.y - p1.y*p2.x;
    return vector3(x,y,z);
  }

  /**
   * @brief calculate the pole direction of two positional classes.
   * @param p1: the first argument of the outer product.
   * @param p2: the second argument of the outer product.
   */
  const direction_cosine
  get_pole(const vector3& p1, const vector3& p2)
  {
    const vector3 n = outer_product(p1,p2);
    if (n.d < __epsilon__)
      throw std::invalid_argument
        ("two vectors are colinear. pole is not defined.");
    return direction_cosine(n);
  }


  /**
   * @brief a helper function to calculate the deflected direction.
   * @param p1: the first anchor point.
   * @param p2: the second anchor point.
   * @param cost: `cos(d)` of `p1` and `p2`.
   * @param cosf1: `cos(f1)` value.
   * @param cosf2: `cos(f2)` value.
   * @param cosfp: `cos(f1+f2)` value.
   * @param cosfm: `cos(f1-f2)` value.
   * @param plus: returns the positive solution if `true`.
   */
  const direction_cosine
  __deflect_helper(const direction_cosine& p1,
                   const direction_cosine& p2,
                   const double cost,
                   const double cosf1, const double cosf2,
                   const double cosfp, const double cosfm)
  {
    const double w2 = (cost-cosfp)*(cosfm-cost);
    if (1.0-cost < __epsilon__)
      throw std::invalid_argument("p1 and p2 are identical.");
    if (w2 < __exact_zero__)
      throw std::range_error("cannot find the solution.");
    const double w = ((cost-cosfp)>0?1.0:-1.0)*std::sqrt(w2);
    const double&& l =
      (p1.l-p2.l*cost)*cosf1+(p2.l-p1.l*cost)*cosf2+(p1.m*p2.n-p1.n*p2.m)*w;
    const double&& m =
      (p1.m-p2.m*cost)*cosf1+(p2.m-p1.m*cost)*cosf2+(p1.n*p2.l-p1.l*p2.n)*w;
    const double&& n =
      (p1.n-p2.n*cost)*cosf1+(p2.n-p1.n*cost)*cosf2+(p1.l*p2.m-p1.m*p2.l)*w;
    return direction_cosine(l,m,n);
  }

  /**
   * @brief obtain the point at a distance of f1 from p1 and
   *        a distance of f2 from p2, respectively.
   * @param p1: the first anchor point.
   * @param p2: the second anchor point.
   * @param f1: the angular distance from `p1`.
   * @param f2: the angular distance from `p2`.
   * @param plus: returns the positive solution if `true`.
   */
  const direction_cosine
  deflect(const direction_cosine& p1,
          const direction_cosine& p2,
          const angle& f1, const angle& f2)
  {
    const double&& cost  = p1.separation_cosine(p2);
    const double&& cosf1 = std::cos(f1.radian);
    const double&& cosf2 = std::cos(f2.radian);
    const double&& cosfp = std::cos((f1+f2).radian);
    const double&& cosfm = std::cos((f1-f2).radian);
    return __deflect_helper(p1,p2,cost,cosf1,cosf2,cosfp,cosfm);
  }

  /**
   * @brief obtain the point at a distance of f1 from p1 and
   *        a distance of f2 from p2, respectively.
   * @param p1: the first anchor point.
   * @param p2: the second anchor point.
   * @param f: the fraction between `p1` and `p2`.
   * @note set `f` in [0,1] for interpolation.
   *       in case that `f<0` or `f>1` an extrapolated value is obtained.
   */
  const direction_cosine
  interp(const direction_cosine& p1,
         const direction_cosine& p2,
         const double f)
  {
    const angle theta = p1.separation(p2);
    const double cost  = p1.separation_cosine(p2);
    const double sint  = std::sqrt(1-cost);
    const double cosf1 = std::cos(theta.radian*f);
    const double sinf1 = std::sin(theta.radian*f);
    const double&& cosf2  = cosf1*cost+sinf1*sint;
    const double&  cosfp  = cost;
    const double&& cos2f1 = cosf1*cosf1-sinf1*sinf1;
    const double&& sin2f1 = 2*cosf1*sinf1;
    const double&& cosfm  = cos2f1*cost+sin2f1*sint;
    return __deflect_helper(p1,p2,cost,cosf1,cosf2,cosfp,cosfm);
  }

  class great_circle {
  public:
    /**
     * @brief generate a great circle on the xy-plane.
     */
    great_circle() : great_circle(0,0,1) {}

    /**
     * @brief generate a great circle with the pole.
     * @param p: a `direction_cosine` instance pointing the pole.
     */
    great_circle(const direction_cosine& p)
      : pole(p) {}

    /**
     * @brief generate a great circle with the pole of (l,m,n).
     * @param l: the l-component of the pole.
     * @param m: the m-component of the pole.
     * @param n: the n-component of the pole.
     */
    great_circle(const double l, const double m, const double n)
      : pole(l,m,n) {}

    /**
     * @brief generate a great circle with the pole of (lon,lat).
     * @param lon: the longitude of the pole.
     * @param lat: the latitude of the pole.
     */
    great_circle(const longitude& lon, const latitude& lat)
      : pole(lon, lat) {}

    /**
     * @brief obtain `cos(d)` to another `great_circle`.
     * @param gc: a `great_circle` instance.
     */
    const angle
    separation_cosine(const great_circle& gc) const
    { return pole.separation_cosine(gc.pole); }

    /**
     * @brief obtain the separation angle to another `great_circle`.
     * @param gc: a `great_circle` instance.
     */
    const angle
    separation(const great_circle& gc) const
    { return pole.separation(gc.pole); }

    /**
     * @brief obtain `cos(d)` to `direction_cosine`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation_cosine(const direction_cosine& p) const
    {
      const double cost = pole.separation_cosine(p);
      return std::sqrt(1-cost*cost);
    }

    /**
     * @brief obtain the separation angle to `direction_cosine`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation(const direction_cosine& p) const
    { return std::acos(separation_cosine(p).radian); }


    /**
     * @brief obtain a foot of the perpendicular from `p`.
     * @param p: a `direction_cosine` instance.
     */
    const direction_cosine
    foot_of(const direction_cosine& p) const
    {
      const double cost = pole.separation_cosine(p);
      if (1.0-cost > __epsilon__) {
        const double sint = std::sqrt(1-cost*cost);
        const double&& l = (p.l-pole.l*cost)*sint;
        const double&& m = (p.m-pole.m*cost)*sint;
        const double&& n = (p.n-pole.n*cost)*sint;
        return direction_cosine(l,m,n);
      } else {
        // `p` and `pole` is almost identical.
        // use (1,0,0) or (0,1,0) instead of `p`.
        const direction_cosine px(1,0,0), py(0,1,0);
        const double&& costx = pole.separation_cosine(px);
        const double&& costy = pole.separation_cosine(py);
        return foot_of(costy>costx?px:py);
      }
    }


    /**
     * @brief dump (x,y,z)-coordinates on the circle.
     * @param N: the number of points (default: 512).
     */
    void dump(const size_t N=64) const
    {
      const direction_cosine p(1,0,0), q(0,1,0);
      const double&& dcosp = pole.separation_cosine(p);
      const double&& dcosq = pole.separation_cosine(q);
      const direction_cosine x = outer_product(pole,(dcosq>dcosp?p:q));
      for (size_t i=0; i<N; i++) {
        const double phi = 2*M_PI/(N-1)*i;
        deflect(x, pole, phi, M_PI_2).dump();
      }
    }

    const direction_cosine pole; /** the pole of the great circle. */
  private:
  };


  class minor_arc : public great_circle {
  public:
    /**
     * @brief construct a `minor_arc` instance from (1,0,0) to (0,1,0).
     */
    minor_arc()
      : minor_arc(dcos{1,0,0},dcos{0,1,0}) {}

    /**
     * @brief construct a `minor_arc` instance from `s` to `e`.
     * @param _s: the starting point of the arc.
     * @param _e: the end pont of the arc.
     * @note throw an exception when
     *         (invalid_argumet): a pole cannot be defined by `s` and `e`.
     */
    minor_arc(const direction_cosine& _s, const direction_cosine& _e)
      : great_circle(get_pole(_s,_e)), s(_s), e(_e) {}

    const direction_cosine s; /** the starting point of the arc. */
    const direction_cosine e; /** the end point of the arc. */
  private:
  };


  class motion_arc : public great_circle {
  public:
    /** disable the constructor without arguments. */
    motion_arc() = delete;

    /**
     * @brief construct a `minor_arc` instance from `s` to `e`.
     * @param _s: the starting point of the arc.
     * @param _e: the end pont of the arc.
     * @note throw an exception when
     *         (invalid_argumet): a pole cannot be defined by `s` and `e`.
     *         (invalid_argumet): timestamps of `s` and `e` are the same.
     */
    motion_arc(const source& _s, const source& _e)
      : great_circle(get_pole(_s,_e)), s(_s), e(_e), dt(_e.t-_s.t),
        h_s1(make_helper(s,e,true)), h_s2(make_helper(s,e,false)),
        h_e1(make_helper(e,s,true)), h_e2(make_helper(e,s,false)),
        p_s1(get_pole(s,h_s1)), p_s2(get_pole(s,h_s2)),
        p_e1(get_pole(e,h_e1)), p_e2(get_pole(e,h_e2)),
        cost_s12(p_s1.separation_cosine(p_s2)),
        cost_e12(p_e1.separation_cosine(p_e2))
    {
      if (std::abs(dt.count()) < 1e-15)
        throw std::invalid_argument
          ("no time difference bwteen two positions.");
      if (__debug__) {
        printf("# s   : "); s.dump();
        printf("# e   : "); e.dump();
        printf("# dt  : %lf s\n", dt.count());
        printf("# h_s1: "); h_s1.dump();
        printf("# h_s2: "); h_s2.dump();
        printf("# h_e1: "); h_e1.dump();
        printf("# h_e2: "); h_e2.dump();
        printf("# p_s1: "); p_s1.dump();
        printf("# p_s2: "); p_s2.dump();
        printf("# p_e1: "); p_e1.dump();
        printf("# p_e2: "); p_e2.dump();
      }
    }

    /**
     * @brief obtain the point after `dT` from `s`.
     * @param dT: duration in second.
     */
    const direction_cosine
    propagate(const sec_t& dT) const;

    /**
     * @brief obtain the point at `T`.
     * @param T: timestamp instance.
     */
    const direction_cosine
    propagate(const timestamp_t& T) const;

    /**
     * @brief calculate `cos(d)` to the point `p` taking into account
     *        the uncertainty of the arc.
     * @param p: a `direction_cosine` instance.
     */
    const double
    separation_cosine(const direction_cosine& p) const
    {
      if (intersect_with(p)) return 1.0;
      const direction_cosine np = get_pole(s,p);
      const double&& cost_np = pole.separation_cosine(np);
      const double&& cost_s1 = p_s1.separation_cosine(np);
      const double&& cost_s2 = p_s2.separation_cosine(np);
      const double&& cost_e1 = p_e1.separation_cosine(np);
      const double&& cost_e2 = p_e2.separation_cosine(np);
      return std::max({cost_np,cost_s1,cost_s2,cost_e1,cost_e2});
    }
    /**
     * @brief calculate the separation angle to the point `p` taking
     *        into account the uncertainty of the arc.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation(const direction_cosine& p) const
    {
      return std::acos(separation_cosine(p));
    }

    /**
     * @brief
     * @param p: a `direction_cosine` instance.
     */
    const bool
    intersect_with(const direction_cosine& p) const
    {
      {
        const direction_cosine ps = get_pole(s,p);
        const double cost_p1 = ps.separation_cosine(p_s1);
        const double cost_p2 = ps.separation_cosine(p_s2);
        const double sint_p1 = std::sqrt(1-cost_p1*cost_p1);
        const double sint_p2 = std::sqrt(1-cost_p2*cost_p2);
        const double cost_pp = cost_p1*cost_p2-sint_p1*sint_p2;
        if (std::abs(cost_pp-cost_s12)<__epsilon__) return true;
      }
      {
        const direction_cosine pe = get_pole(e,p);
        const double cost_p1 = pe.separation_cosine(p_e1);
        const double cost_p2 = pe.separation_cosine(p_e2);
        const double sint_p1 = std::sqrt(1-cost_p1*cost_p1);
        const double sint_p2 = std::sqrt(1-cost_p2*cost_p2);
        const double cost_pp = cost_p1*cost_p2-sint_p1*sint_p2;
        if (std::abs(cost_pp-cost_e12)<__epsilon__) return true;
      }
      return false;
    }

    const bool
    colinear_with(const great_circle& gc, const angle& tol = degree(5.0));

    const source s; /** the starting point of the arc. */
    const source e; /** the end point of the arc. */
    const sec_t dt; /** the time separation between `s` and `e` */
  private:
    const direction_cosine h_s1; /** the first helper point for `s`. */
    const direction_cosine h_s2; /** the second helper point for `s`. */
    const direction_cosine h_e1; /** the first helper point for `e`. */
    const direction_cosine h_e2; /** the second helper point for `e`. */
    const direction_cosine p_s1; /** the first helper pole for `s`. */
    const direction_cosine p_s2; /** the second helper pole for `s`. */
    const direction_cosine p_e1; /** the first helper pole for `e`. */
    const direction_cosine p_e2; /** the second helper pole for `e`. */
    const double cost_s12; /** a helper varialbe for `intersect_with`. */
    const double cost_e12; /** a helper variable for `intersect_with`. */

    /**
     * @brief generate a helper point.
     * @param from: the origin of the line segment (`s` or `e`).
     * @param to: the destination of the line segment (`s` or `e`).
     * @param parity: the positive solution if `true`.
     */
    const direction_cosine
    make_helper(const source& from, const source& to, const bool parity)
    {
      const angle theta = from.separation(to);
      const angle delta = to.s;
      const angle phi = radian(std::hypot(theta.radian,delta.radian));
      return deflect(from, to, phi, (parity?1.0:-1.0)*delta);
    }
  };
}

#endif  // __GCXMLIB_H_INCLUDE
