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
  constexpr double __epsilon__ = 1e-12;
  constexpr double __exact_zero__ = 0.0;
  constexpr double radian_to_degree = 180./M_PI;
  constexpr double degree_to_radian = M_PI/180.;
  constexpr double radian_to_arcmin = 60.*180./M_PI;
  constexpr double arcmin_to_radian = M_PI/180./60.;
  constexpr double radian_to_arcsec = 3600.*180./M_PI;
  constexpr double arcsec_to_radian = M_PI/180./3600.;

  using sec_t = std::chrono::duration<double>;
  using default_clock = std::chrono::system_clock;
  using timestamp_t = std::chrono::time_point<default_clock>;

  /**
   * @brief return the current time using `default_clock`.
   */
  const auto
  now() { return default_clock::now(); }

  /**
   * @brief a helper function to advance the timestamp by `dt`.
   * @param t0: the `timestamp_t` instance of the time origin.
   * @param dt: the `chrono::duration` instance.
   */
  template<typename duration_t>
  const auto
  advance_timestamp(const timestamp_t& t0, const duration_t& dt)
  {
    return t0 + std::chrono::duration_cast<default_clock::duration>(dt);
  }

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
  using angle = base_angle<angle_range::zero_to_twopi>;
  /** `longitude` is an alias to `angle`. */
  using longitude = base_angle<angle_range::zero_to_twopi>;
  /** `latitude` is defined within [-pi/2,pi/2]. */
  using latitude = base_angle<angle_range::minus_pi_2_to_pi_2>;

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
     * @brief calculate the outer product against `v`.
     * @param v: the second argument of the outer product.
     */
    const vector3
    outer_product(const vector3& v) const
    {
      const double&& nx = y*v.z - z*v.y;
      const double&& ny = z*v.x - x*v.z;
      const double&& nz = x*v.y - y*v.x;
      return vector3(nx,ny,nz);
    }

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

    /**
     * @brief calculate the normal vector of the plane.
     * @param v: the second argument of the outer product.
     */
    const direction_cosine
    get_pole(const vector3& v) const
    {
      const vector3 n = outer_product(v);
      if (n.d < __epsilon__)
        throw std::invalid_argument
          ("two vectors are colinear. pole is not defined.");
      return direction_cosine(n);
    }

    /**
     * @brief obtain the point at a distance of f1 from p1 and
     *        a distance of f2 from p2, respectively.
     * @param q: the anchor point.
     * @param f1: the angular distance from this point.
     * @param f2: the angular distance from `q`.
     */
    const direction_cosine
    pivot(const direction_cosine& q, const angle& f1, const angle& f2) const
    {
      const double&& cosd  = separation_cosine(q);
      const double&& cosf1 = std::cos(f1.radian);
      const double&& cosf2 = std::cos(f2.radian);
      const double&& cosfp = std::cos((f1+f2).radian);
      const double&& cosfm = std::cos((f1-f2).radian);
      return __pivot_helper(q,cosd,cosf1,cosf2,cosfp,cosfm);
    }

    /**
     * @brief obtain the point extended toward `q` by fraction `f`.
     * @param q: the anchor point.
     * @param f: the fraction of the length to `q`.
     * @note interpolation for `f` between [0,1], otherwise extrapolation.
     */
    const direction_cosine
    extend_to(const direction_cosine& q, const double f) const
    {
      const angle theta = separation(q);
      const double cosd  = separation_cosine(q);
      const double sind  = std::sqrt(1-cosd);
      const double cosf1 = std::cos(theta.radian*f);
      const double sinf1 = std::sin(theta.radian*f);
      const double&& cosf2  = cosf1*cosd+sinf1*sind;
      const double&  cosfp  = cosd;
      const double&& cos2f1 = cosf1*cosf1-sinf1*sinf1;
      const double&& sin2f1 = 2*cosf1*sinf1;
      const double&& cosfm  = cos2f1*cosd+sin2f1*sind;
      return __pivot_helper(q,cosd,cosf1,cosf2,cosfp,cosfm);
    }

    const double& l; /** l-element of direction cosine (reference to x) */
    const double& m; /** m-element of direction cosine (reference to y) */
    const double& n; /** n-element of direction cosine (reference to z) */
    const longitude lon; /** longitude angle. */
    const latitude lat;  /** latitude angle. */

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
    /** calculate longitude from (l,m,n) */
    const longitude
    calc_longitude()
    { return longitude(std::atan2(m,l));}
    /** calculate latitude from (l,m,n) */
    const latitude
    calc_latitude()
    { return latitude(std::asin(n)); }
    /**
     * @brief a helper function to calculate the deflected direction.
     * @param q: the anchor point.
     * @param cosd: `cos(d)` to `q`.
     * @param cosf1: `cos(f1)` value.
     * @param cosf2: `cos(f2)` value.
     * @param cosfp: `cos(f1+f2)` value.
     * @param cosfm: `cos(f1-f2)` value.
     * @param plus: returns the positive solution if `true`.
     */
    const direction_cosine
    __pivot_helper(const direction_cosine& q, const double cosd,
                   const double cosf1, const double cosf2,
                   const double cosfp, const double cosfm) const
    {
      const double w2 = (cosd-cosfp)*(cosfm-cosd);
      if (1.0-cosd < __epsilon__)
        throw std::invalid_argument("p1 and p2 are identical.");
      if (w2 < __exact_zero__)
        throw std::range_error("cannot find the solution.");
      const double w = ((cosd-cosfp)>0?1.0:-1.0)*std::sqrt(w2);
      const double&& pl =
        (l-q.l*cosd)*cosf1+(q.l-l*cosd)*cosf2+(m*q.n-n*q.m)*w;
      const double&& pm =
        (m-q.m*cosd)*cosf1+(q.m-m*cosd)*cosf2+(n*q.l-l*q.n)*w;
      const double&& pn =
        (n-q.n*cosd)*cosf1+(q.n-n*cosd)*cosf2+(l*q.m-m*q.l)*w;
      return direction_cosine(pl,pm,pn);
    }
  };
  /** define _dcos_ as a shorthand of direction_cosine. */
  using dcos = direction_cosine;


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
           const timestamp_t& _t, const angle& _s = arcsec(1.0))
      : direction_cosine(_x, _y, _z), t(_t), s(_s) {}

    /**
     * @brief construct a `source` instance with (lon,lat,t)
     * @note set `s` 1 arcsec if not assigned.
     */
    source(const longitude& _lon, const latitude& _lat,
           const timestamp_t& _t, const angle& _s = arcsec(1.0))
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
     * @brief obtain the source extended toward `q` by fraction `f`.
     * @param q: the anchor point.
     * @param f: the fraction of the length to `q`.
     * @note interpolation for `f` between [0,1], otherwise extrapolation.
     *       this method is defined later since it depends on `motion_arc`.
     */
    const source
    extend_to(const source& q, const double f) const;

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
    { return (default_clock::now()); }
  };

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
    const double
    separation_cosine(const direction_cosine& p) const
    {
      const double cosd = pole.separation_cosine(p);
      return std::sqrt(1-cosd*cosd);
    }

    /**
     * @brief obtain the separation angle to `direction_cosine`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation(const direction_cosine& p) const
    { return std::acos(separation_cosine(p)); }


    /**
     * @brief obtain a foot of the perpendicular from `p`.
     * @param p: a `direction_cosine` instance.
     */
    const direction_cosine
    foot_of(const direction_cosine& p) const
    {
      const double cosd = pole.separation_cosine(p);
      if (1.0-cosd > __epsilon__) {
        const double sind = std::sqrt(1-cosd*cosd);
        const double&& l = (p.l-pole.l*cosd)*sind;
        const double&& m = (p.m-pole.m*cosd)*sind;
        const double&& n = (p.n-pole.n*cosd)*sind;
        return direction_cosine(l,m,n);
      } else {
        // `p` and `pole` is almost identical.
        // use (1,0,0) or (0,1,0) instead of `p`.
        const direction_cosine px(1,0,0), py(0,1,0);
        const double&& cosdx = pole.separation_cosine(px);
        const double&& cosdy = pole.separation_cosine(py);
        return foot_of(cosdy>cosdx?px:py);
      }
    }


    /**
     * @brief dump (x,y,z)-coordinates on the circle.
     * @param N: the number of points (default: 64).
     */
    void dump(const size_t N=64) const
    { dump_with_pole(pole, N); }

    const direction_cosine pole; /** the pole of the great circle. */
  protected:
    /**
     * @brief dump (x,y,z)-coordinates on the circle.
     * @param _pole: the pole of the great circle.
     * @param N: the number of points.
     */
    void
    dump_with_pole(const direction_cosine& _pole, const size_t N) const
    {
      const direction_cosine p(1,0,0), q(0,1,0);
      const double&& dcosp = _pole.separation_cosine(p);
      const double&& dcosq = _pole.separation_cosine(q);
      const direction_cosine x = _pole.outer_product(dcosq>dcosp?p:q);
      for (size_t i=0; i<N; i++) {
        const double phi = 2*M_PI/(N-1)*i;
        x.pivot(_pole, phi, M_PI_2).dump();
      }
    }

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
      : great_circle(_s.get_pole(_e)), s(_s), e(_e),
        cosd_se(_s.separation_cosine(_e)) {}

    /**
     * @brief calculate `cos(d)` between the arc and the point `p`.
     * @param p: a `direction_cosine` instance.
     */
    const double
    distance_cosine(const direction_cosine& p) const
    {
      const direction_cosine ft = foot_of(p);
      const double&& cosd_ps = s.separation_cosine(ft);
      const double&& cosd_pe = e.separation_cosine(ft);
      const double&& sind_ps = std::sqrt(1-cosd_ps*cosd_ps);
      const double&& sind_pe = std::sqrt(1-cosd_pe*cosd_pe);
      const double&& cosd = cosd_ps*cosd_pe-sind_ps*sind_pe;
      const double&& sind = cosd_ps*sind_pe+sind_ps*cosd_pe;
      if (std::abs(cosd-cosd_se) < __epsilon__ && sind > 0) {
        return separation_cosine(p);
      } else {
        const double&& sep_s = s.separation_cosine(p);
        const double&& sep_e = e.separation_cosine(p);
        return std::max(sep_s,sep_e);
      }
    }

    /**
     * @brief calculate the distance between the arc and the point `p`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    distance(const direction_cosine& p) const
    {
      return std::acos(distance_cosine(p));
    }

    /**
     * @brief dump (x,y,z)-coordinates of the arc.
     * @param N: the number of points (default: 64).
     */
    void
    dump_arc(const size_t N=64) const
    {
      for (size_t i=0; i<N; i++) {
        const double&& f=1.0/(N-1)*i;
        const direction_cosine p = s.extend_to(e,f);
        p.dump();
      }
    }

    const direction_cosine s; /** the starting point of the arc. */
    const direction_cosine e; /** the end point of the arc. */
  private:
    const double cosd_se; /** `cos(d)` between `s` and `e`. */
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
      : great_circle(_s.get_pole(_e)), s(_s), e(_e), dt(_e.t-_s.t),
        h_s1(make_helper(s,e,true)), h_s2(make_helper(s,e,false)),
        h_e1(make_helper(e,s,true)), h_e2(make_helper(e,s,false)),
        p_s1(s.get_pole(h_s1)), p_s2(s.get_pole(h_s2)),
        p_e1(e.get_pole(h_e1)), p_e2(e.get_pole(h_e2)),
        cosd_s12(p_s1.separation_cosine(p_s2)),
        cosd_e12(p_e1.separation_cosine(p_e2)),
        arc_s(minor_arc(p_s1,p_s2)), arc_e(minor_arc(p_e2,p_e1))
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
     * @brief calculate `cos(d)` to the great circle `gc` taking into account
     *        the uncertainty of the arc.
     * @param gc: a `great_circle` instance.
     */
    const double
    separation_cosine(const great_circle& gc) const
    {
      const double&& ds = arc_s.distance_cosine(gc.pole);
      const double&& de = arc_e.distance_cosine(gc.pole);
      printf("# sep %lf %lf\n", ds, de);
      return std::max(ds, de);
    }
    /**
     * @brief calculate the separation angle to the great circle `gc`
     *        taking into account the uncertainty of the arc.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation(const great_circle& gc) const
    {
      return std::acos(separation_cosine(gc));
    }

    /**
     * @brief calculate `cos(d)` to the point `p` taking into account
     *        the uncertainty of the arc.
     * @param p: a `direction_cosine` instance.
     */
    const double
    separation_cosine(const direction_cosine& p) const
    {
      if (intersect_with(p)) return 1.0;
      const double&& c_np = std::abs(pole.separation_cosine(p));
      const double&& c_s1 = std::abs(p_s1.separation_cosine(p));
      const double&& c_s2 = std::abs(p_s2.separation_cosine(p));
      const double&& c_e1 = std::abs(p_e1.separation_cosine(p));
      const double&& c_e2 = std::abs(p_e2.separation_cosine(p));
      const double&& c = std::min({c_np,c_s1,c_s2,c_e1,c_e2});
      return std::sqrt(1.0-c*c);
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
     * @brief check if the arc intersects with the position `p` taking
     *        into account the uncertainties of the end points.
     * @param p: a `direction_cosine` instance.
     */
    const bool
    intersect_with(const direction_cosine& p) const
    {
      {
        const direction_cosine ps = s.get_pole(p);
        const double&& cosd_p1 = ps.separation_cosine(p_s1);
        const double&& cosd_p2 = ps.separation_cosine(p_s2);
        const double&& sind_p1 = std::sqrt(1-cosd_p1*cosd_p1);
        const double&& sind_p2 = std::sqrt(1-cosd_p2*cosd_p2);
        const double&& cosd = cosd_p1*cosd_p2-sind_p1*sind_p2;
        if (std::abs(cosd-cosd_s12)<__epsilon__) return true;
      }
      {
        return false;
        const direction_cosine pe = e.get_pole(p);
        const double&& cosd_p1 = pe.separation_cosine(p_e1);
        const double&& cosd_p2 = pe.separation_cosine(p_e2);
        const double&& sind_p1 = std::sqrt(1-cosd_p1*cosd_p1);
        const double&& sind_p2 = std::sqrt(1-cosd_p2*cosd_p2);
        const double&& cosd = cosd_p1*cosd_p2-sind_p1*sind_p2;
        if (std::abs(cosd-cosd_e12)<__epsilon__) return false;
      }
      return false;
    }

    /**
     * @brief check if the arc intersects with the arc `arc` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `minor_arc` instance.
     */
    const bool
    intersect_with(const minor_arc& arc) const
    {
      return intersect_with(arc.s) || intersect_with(arc.e);
    }

    /**
     * @brief check if the arc intersects with the arc `arc` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `motion_arc` instance.
     */
    const bool
    intersect_with(const motion_arc& arc) const
    {
      return intersect_with(arc.s) || intersect_with(arc.e);
    }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param gc: a `great_circle` instance.
     */
    const bool
    colinear_with(const great_circle& gc,
                  const angle& tol = degree(5.0)) const
    {
      if (tol.degree > 90.0)
        throw std::invalid_argument("invalid tolerance value.");
      const double&& cosd = separation_cosine(gc);
      return cosd >= std::cos(tol.radian);
    }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `minor_arc` instance.
     */
    const bool
    colinear_with(const minor_arc& arc,
                  const angle& tol = degree(5.0)) const
    {
      if (tol.degree > 90.0)
        throw std::invalid_argument("invalid tolerance value.");
      if (!intersect_with(arc)) throw false;
      const double&& cosd = separation_cosine(arc);
      return cosd >= std::cos(tol.radian);
    }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `motion_arc` instance.
     */
    const bool
    colinear_with(const motion_arc& arc,
                  const angle& tol = degree(5.0)) const
    {
      if (tol.degree > 90.0)
        throw std::invalid_argument("invalid tolerance value.");
      if (!intersect_with(arc)) throw false;
      const double&& cosd = separation_cosine(arc);
      return cosd >= std::cos(tol.radian);
    }

    /**
     * @brief return the uncertainty at the foot of `q`.
     * @param q: a `direction_cosine` instance.
     * @param skip: skip execution of `foot_of` if true.
     */
    const angle
    error_at(const direction_cosine& q, const bool skip=true) const
    {
      const direction_cosine ft = (skip)?foot_of(q):q;
      const double&& s1 = p_s1.separation_cosine(ft);
      const double&& s2 = p_s2.separation_cosine(ft);
      const double&& e1 = p_e1.separation_cosine(ft);
      const double&& e2 = p_e2.separation_cosine(ft);
      const double&& d_s1 = std::sqrt(1-s1*s1);
      const double&& d_s2 = std::sqrt(1-s2*s2);
      const double&& d_e1 = std::sqrt(1-e1*e1);
      const double&& d_e2 = std::sqrt(1-e2*e2);
      return std::acos(std::min({d_s1,d_s2,d_e1,d_e2}));
    }

    /**
     * @brief dump (x,y,z)-coordinates of the arc.
     * @param N: the number of points (default: 64).
     */
    void
    dump_arc(const size_t N=64) const
    {
      for (size_t i=0; i<N; i++) {
        const double&& f=1.0/(N-1)*i;
        const direction_cosine p = s.extend_to(e,f);
        p.dump();
      }
    }

    /**
     * @brief dump (x,y,z)-coordinates on the uncertainty circles.
     * @param N: the number of points (default: 64).
     */
    void dump_error(const size_t N=64) const
    {
      dump_with_pole(p_s1, N);
      printf("\n");
      dump_with_pole(p_s2, N);
      printf("\n");
      dump_with_pole(p_e1, N);
      printf("\n");
      dump_with_pole(p_e2, N);
    }

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
    const double cosd_s12; /** a helper varialbe for `intersect_with`. */
    const double cosd_e12; /** a helper variable for `intersect_with`. */
    const minor_arc arc_s; /** a helper variable for `separation_cosine`. */
    const minor_arc arc_e; /** a helper variable for `separation_cosine`. */

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
      return from.pivot(to, phi, (parity?1.0:-1.0)*delta);
    }
  };

  const source
  source::extend_to(const source& q, const double f) const
  {
    const timestamp_t eT = advance_timestamp(t, f*(q.t-t));
    const direction_cosine& ep = direction_cosine::extend_to(q,f);
    const double&& es = motion_arc(*this, q).error_at(ep);
    return source(ep.l,ep.m,ep.n,eT,es);
  }
}

#endif  // __GCXMLIB_H_INCLUDE
