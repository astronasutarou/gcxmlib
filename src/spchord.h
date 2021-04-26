/**
 * @file spchord.h
 * @brief header file for the spchord library
 * @author Ryou Ohsawa
 * @year 2021
 */
#ifndef __SPCHORD_H_INCLUDE
#define __SPCHORD_H_INCLUDE
#define _USE_MATH_DEFINES

#include <cstdio>
#include <cmath>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>
#include <set>
#include <stdexcept>


namespace spchord {
  using namespace std::chrono_literals;

  constexpr bool __debug__ = false;
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
    /**
     * @brief construct an `base_angle` instance.
     * @param[in] _r: angle value in radian.
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
     * @param[in] ang: a `base_angle` instance.
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
     * @param[in] ang: a `base_angle` intance.
     * @param[in] val: a floating point value.
     */
    friend const base_angle<_range>
    operator+(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian+val); }
    /**
     * @brief `add` operation with a floating point value.
     * @param[in] val: a floating point value.
     * @param[in] ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator+(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val+ang.radian); }
    /**
     * @brief `subtract` operation with a floating point value.
     * @param[in] ang: a `base_angle` intance.
     * @param[in] val: a floating point value.
     */
    friend const base_angle<_range>
    operator-(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian-val); }
    /**
     * @brief `subtract` operation with a floating point value.
     * @param[in] val: a floating point value.
     * @param[in] ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator-(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val-ang.radian); }
    /**
     * @brief `multiply` operation with a floating point value.
     * @param[in] ang: a `base_angle` intance.
     * @param[in] val: a floating point value.
     */
    friend const base_angle<_range>
    operator*(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian*val); }
    /**
     * @brief `multiply` operation with a floating point value.
     * @param[in] val: a floating point value.
     * @param[in] ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator*(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(ang.radian*val); }
    /**
     * @brief `divide` operation with a floating point value.
     * @param[in] ang: a `base_angle` intance.
     * @param[in] val: a floating point value.
     */
    friend const base_angle<_range>
    operator/(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian/val); }
    /**
     * @brief `divide` operation with a floating point value.
     * @param[in] val: a floating point value.
     * @param[in] ang: a `base_angle` intance.
     */
    friend const base_angle<_range>
    operator/(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val/ang.radian); }

    /**
     * @brief `add` operation with another `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const base_angle<_range>
    operator+(const base_angle<__range>& ang) const
    { return base_angle<_range>(radian+ang.radian); }
    /**
     * @brief `subtract` operation with another `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const base_angle<_range>
    operator-(const base_angle<__range>& ang) const
    { return base_angle<_range>(radian-ang.radian); }

    /**
     * @brief equality operator to a `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator==(const base_angle<__range>& ang) const
    { return (radian == ang.radian); }
    /**
     * @brief inequality operator to a `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator!=(const base_angle<__range>& ang) const
    { return (radian != ang.radian); }
    /**
     * @brief lesser operator to a `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator<(const base_angle<__range>& ang) const
    { return (radian < ang.radian); }
    /**
     * @brief lesser-or-equal operator to a `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator<=(const base_angle<__range>& ang) const
    { return (radian <= ang.radian); }
    /**
     * @brief greater operator to a `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator>(const base_angle<__range>& ang) const
    { return (radian > ang.radian); }
    /**
     * @brief greater-or-equal operator to a `base_angle` instance.
     * @param[in] ang: another `base_angle` intance.
     */
    template <angle_range __range>
    const bool
    operator>=(const base_angle<__range>& ang) const
    { return (radian >= ang.radian); }

  private:
    /**
     * @brief wrap the angle within the domain of `base_angle`.
     * @param[in] ang: angle value in radian.
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
    inner_product(const vector3& p) const
    { return p.x*x + p.y*y + p.z*z; }


    /**
     * @brief return `cos(d)` where `d` is the angular separation.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    separation_cosine(const vector3& p) const
    { return inner_product(p)/p.d/d; }

    /**
     * @brief return the separation angle from `p` in radian.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    separation(const vector3& p) const
    { return std::acos(separation_cosine(p)); }

    /**
     * @brief return the separation angle from `p` in arcsecond.
     * @param[in] p: an instance of `vector3` class.
     */
    const double
    separation_arcsec(const vector3& p) const
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
     * @param[in] _l: x-coordinate.
     * @param[in] _m: y-coordinate.
     * @param[in] _n: z-coordinate.
     */
    direction_cosine(const double _l, const double _m, const double _n)
      : vector3(nx(_l,_m,_n),ny(_l,_m,_n),nz(_l,_m,_n)), l(x), m(y), n(z),
        lon(calc_longitude()), lat(calc_latitude())
    {}

    /**
     * @brief construct a `direction_cosine` instance with `vector3`.
     * @parma[in] p: an instance of `vector3` class.
     */
    direction_cosine(const vector3& p)
      : direction_cosine(p.x, p.y, p.z)
    {}

    /**
     * @brief construct a `direction_cosine` instance with (lon,lat).
     * @param[in] lon: a `longitude` instance.
     * @param[in] lat: a `latitude` instance.
     */
    direction_cosine(const latitude& _lon, const latitude& _lat)
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
    /**
     * @brief construct a `source` instance initialized with (1,0,0).
     */
    source() : source(1.0, 0.0, 0.0) {}

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
      printf("%.5lf %.5lf %.5lf %s\n", x, y, z, ss.str().c_str());
    }


    /**
     * @brief return `true` if another point is inside the range.
     * @param[in] p: another positional instance.
     * @param[in] range: an `angle` instance.
     * @note acceptable positional classes are as follows:
     *         - vector3
     *         - dirction_cosine
     *         - source
     */
    template<class T>
    const bool
    neighbor_to(const T& p, const angle& range) const
    {
      const double cosine = std::cos(range.radian);
      const double sepcos = separation_cosine(p);
      return sepcos >= cosine;
    }

    /**
     * @brief return `true` if another point is inside the range.
     * @param[in] p: another positional instance.
     * @param[in] range: an `angle` instance.
     * @param[in] trange: a time duration.
     * @note acceptable positional classes are as follows:
     *         - vector3
     *         - dirction_cosine
     *         - source
     */
    template<class T>
    const bool
    match(const T& p, const angle& range, const sec_t& trange) const
    {
      const double cosine = std::cos(range.radian);
      const double tsep = std::abs(static_cast<sec_t>(t-p.t).count());
      const double sepcos = separation_cosine(p);
      printf("tsep = %lf\n", tsep);
      return (sepcos >= cosine) && (tsep < trange.count());
    }

    /**
     * @brief return `true` if another point is inside the uncertainty.
     * @param[in] p: another positional instance.
     */
    template<class T>
    const bool
    neighbor_to(const T& p) const
    { return neighbor_to(p, s.radian); }

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
   * @brief return `true` if another point is inside the uncertainty.
   * @param[in] p: another positional instance.
   * @note `range` is set the summation of the both uncertainties
   *       if `p` is an instance of `source`. as `range`.
   */
  template<> const bool
  source::neighbor_to<source>(const source& p) const
  { return neighbor_to(p, s.radian+p.s.radian); }
}

#endif  // __SPCHORD_H_INCLUDE
