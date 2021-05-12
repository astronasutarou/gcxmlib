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
#include <array>
#include <set>
#include <stdexcept>
#include <initializer_list>
#include <memory>


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
   * @brief arccosine with a clip function.
   * @param arg: an argument of std::acos.
   */
  template<typename T>
  const double _acos(const T& arg)
  { return std::acos(std::max(-1.0,std::min((double)arg,1.0))); }


  /**
   * @brief return the current time using `default_clock`.
   */
  const timestamp_t
  now() { return default_clock::now(); }


  /**
   * @brief generate a `timestamp_t` instance with microsecond resolution.
   * @param year: calender year
   * @param month: calender month
   * @param day: calender day
   * @param hour: hour
   * @param minute: minute
   * @param second: second
   * @param microsecond: microsecond
   * @note local timezone settings are ignored.
   */
  const timestamp_t
  generate_timestamp
  (const int32_t year, const int32_t month, const int32_t day,
   const int32_t hour, const int32_t minute, const int32_t second,
   const int32_t microsecond)
  {
    // check the ranges.
    if (month<0 || month>13)
      throw std::range_error("month range exceeds.");
    if (day<0 || day>31)
      throw std::range_error("day range exceeds.");
    if (hour<0 || hour>23)
      throw std::range_error("hour range exceeds.");
    if (minute<0 || minute>60)
      throw std::range_error("minute range exceeds.");
    if (second<0 || second>60)
      throw std::range_error("second range exceeds.");
    if (microsecond<0 || microsecond > 1000000)
      throw std::range_error("microsecond range exceeds.");
    // store the current timezone settings.
    char* tz = getenv("TZ");
    struct tm ts = {
      .tm_sec   = second,      // [0..60]
      .tm_min   = minute,      // [0..59]
      .tm_hour  = hour,        // [0..23]
      .tm_mday  = day,         // [1..31]
      .tm_mon   = month - 1,   // [0..11]
      .tm_year  = year - 1900  // year since 1990
    };
    // clear the timezone settings.
    setenv("TZ","",1); tzset();
    timestamp_t t0 = default_clock::from_time_t(std::mktime(&ts));
    // restore the timezone settings.
    (tz)?setenv("TZ",tz,1):unsetenv("TZ"); tzset();
    return t0+std::chrono::microseconds(microsecond);
  }

  /**
   * @brief convert a `timestamp_t` instance into std::string.
   * @param ts: a `timestamp_t` instance.
   */
  const std::string
  timestamp_to_string(const timestamp_t& ts)
  {
    std::stringstream ss;
    const auto duration  = ts.time_since_epoch();
    // modification term for datetime before 1990
    const auto mod = duration.count()>0?0:1;
    const auto subsecond =
      (std::chrono::duration_cast<std::chrono::microseconds>(duration)
       % std::chrono::seconds{1}) + std::chrono::seconds{mod};
    const std::time_t ti = std::chrono::system_clock::to_time_t(ts)-mod;
    ss << std::put_time(std::gmtime(&ti), "%FT%T") << "."
       << std::setfill('0') << std::setw(6) << subsecond.count();
    return ss.str();
  }

  /**
   * @brief a helper function to advance the timestamp by `dt`.
   * @param t0: the `timestamp_t` instance of the time origin.
   * @param dt: the `chrono::duration` instance.
   */
  template<typename duration_t>
  const timestamp_t
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
     * @brief construct a `base_angle` instance.
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
     * @brief construct a `base_angle` instance from another `base_angle`.
     * @param ang: a `base_angle` instance.
     */
    template<angle_range __range>
    base_angle(const base_angle<__range>& ang)
      : base_angle(ang.radian) {}

    const double radian; /** angle in radian */
    const double degree; /** angle in degree */
    const double arcmin; /** angle in arcmin */
    const double arcsec; /** angle in arcsec */

    /** implicit conversion to `double` returns `radian`. */
    operator double() const { return radian; }

    /** `unary minus` operator. */
    const base_angle<_range>
    operator-() const
    { return base_angle<_range>(-radian); }

    /** `add` operation with a floating point value. */
    friend const base_angle<_range>
    operator+(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian+val); }
    /** `add` operation with a floating point value. */
    friend const base_angle<_range>
    operator+(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val+ang.radian); }
    /** `subtract` operation with a floating point value. */
    friend const base_angle<_range>
    operator-(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian-val); }
    /** `subtract` operation with a floating point value. */
    friend const base_angle<_range>
    operator-(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val-ang.radian); }
    /** `multiply` operation with a floating point value. */
    friend const base_angle<_range>
    operator*(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian*val); }
    /** `multiply` operation with a floating point value. */
    friend const base_angle<_range>
    operator*(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(ang.radian*val); }
    /** `divide` operation with a floating point value. */
    friend const base_angle<_range>
    operator/(const base_angle<_range>& ang, const double val)
    { return base_angle<_range>(ang.radian/val); }
    /** `divide` operation with a floating point value. */
    friend const base_angle<_range>
    operator/(const double val, const base_angle<_range>& ang)
    { return base_angle<_range>(val/ang.radian); }

    /** `add` operation with another `base_angle` instance. */
    template <angle_range __range>
    const base_angle<_range>
    operator+(const base_angle<__range>& ang) const
    { return base_angle<_range>(radian+ang.radian); }
    /** `subtract` operation with another `base_angle` instance. */
    template <angle_range __range>
    const base_angle<_range>
    operator-(const base_angle<__range>& ang) const
    { return base_angle<_range>(radian-ang.radian); }

    /** `equality` operator to a `base_angle` instance. */
    template <angle_range __range>
    const bool
    operator==(const base_angle<__range>& ang) const
    { return (radian == ang.radian); }
    /** `inequality` operator to a `base_angle` instance. */
    template <angle_range __range>
    const bool
    operator!=(const base_angle<__range>& ang) const
    { return (radian != ang.radian); }
    /** `lesser` operator to a `base_angle` instance. */
    template <angle_range __range>
    const bool
    operator<(const base_angle<__range>& ang) const
    { return (radian < ang.radian); }
    /** `lesser-or-equal` operator to a `base_angle` instance. */
    template <angle_range __range>
    const bool
    operator<=(const base_angle<__range>& ang) const
    { return (radian <= ang.radian); }
    /** `greater` operator to a `base_angle` instance. */
    template <angle_range __range>
    const bool
    operator>(const base_angle<__range>& ang) const
    { return (radian > ang.radian); }
    /** `greater-or-equal` operator to a `base_angle` instance. */
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
    /** user-defined literal for radian. */
    const angle operator"" rad(long double ang)
    { return radian(ang); }
    /** user-defined literal for degree. */
    const angle operator"" deg(long double ang)
    { return degree(ang); }
    /** user-defined literal for arcminute. */
    const angle operator"" amin(long double ang)
    { return arcmin(ang); }
    /** user-defined literal for arcsecond. */
    const angle operator"" asec(long double ang)
    { return arcsec(ang); }
  }


  class vector3 {
  public:
    /** disable the constructor without arguments. */
    vector3() = delete;

    /** copy constructor */
    vector3(const vector3& copy)
      : vector3(copy.x,copy.y,copy.z) {}

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
     * @param v: an instance of `vector3` class.
     */
    const double
    inner_product(const vector3& v) const
    { return v.x*x + v.y*y + v.z*z; }

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
     * @param v: an instance of `vector3` class.
     */
    const double
    separation_cosine(const vector3& v) const
    { return inner_product(v)/v.d/d; }

    /**
     * @brief return the separation angle from `p` in radian.
     * @param v: an instance of `vector3` class.
     */
    const angle
    separation(const vector3& v) const
    { return _acos(separation_cosine(v)); }

    /**
     * @brief dump all the elements to stdout.
     */
    void dump() const
    { printf("%+.5lf %+.5lf %+.5lf\n", x, y, z); }

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
     * @param v: an instance of `vector3` class.
     */
    direction_cosine(const vector3& v)
      : direction_cosine(v.x, v.y, v.z)
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
     * @brief obtain the point extended toward `q` by separation `d`.
     * @param q: the anchor point.
     * @param d: the separation angle from this point.
     */
    const direction_cosine
    extend_to(const direction_cosine& q, const angle& d) const
    {
      const double cosd  = separation_cosine(q);
      const double sind  = std::sqrt(1-cosd*cosd);
      const double cosf1 = std::cos(d.radian);
      const double sinf1 = std::sin(d.radian);
      const double&& cosf2  = cosf1*cosd+sinf1*sind;
      const double& cosfp  = cosd; // make sure `w` equals zero.
      return __pivot_helper(q,cosd,cosf1,cosf2,cosfp,cosfp);
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
      const angle theta = separation(q)*f;
      return extend_to(q, theta);
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
      if (__debug__) {
        printf("# direction_cosine::pivot_helper\n");
        printf("#   w2    :%+g\n", w2);
        printf("#   cosd  :%+g\n", cosd);
        printf("#   cosf1 :%+g\n", cosf1);
        printf("#   cosf2 :%+g\n", cosf2);
        printf("#   cosfp :%+g\n", cosfp);
        printf("#   cosfm :%+g\n", cosfm);
      }
      if (1.0-cosd < __epsilon__)
        throw std::invalid_argument("p1 and p2 are almost identical.");
      if (w2 < -__epsilon__)
        throw std::range_error("cannot find the solution.");
      const double w = ((cosd-cosfm)>0?1.0:-1.0)*std::sqrt(std::max(w2,0.));
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


  class footprint : public direction_cosine {
  public:
    /** disable the constructor without arguments. */
    footprint() = delete;

    /** copy constractor */
    footprint(const footprint& copy)
      : footprint(copy.l,copy.m,copy.n,copy.t,copy.s) {}

    /**
     * @brief construct a `footprint` instance with (l,m,n)
     * @note set `t` and `s` the system time and 1 arcsec, respectivly.
     */
    footprint(const double _l, const double _m, const double _n)
      : direction_cosine(_l, _m, _n), t(now()), s(arcsec(1.0)) {}

    /**
     * @brief construct a `footprint` instance with (l,m,n,t)
     * @note set `s` 1 arcsec if not assigned.
     */
    footprint(const double _l, const double _m, const double _n,
           const timestamp_t& _t, const angle& _s = arcsec(1.0))
      : direction_cosine(_l, _m, _n), t(_t), s(_s) {}

    /**
     * @brief construct a `footprint` instance with (lon,lat,t)
     * @note set `s` 1 arcsec if not assigned.
     */
    footprint(const longitude& _lon, const latitude& _lat,
           const timestamp_t& _t, const angle& _s = arcsec(1.0))
      : direction_cosine(_lon, _lat), t(_t), s(_s) {}

    /**
     * @brief return `true` if another point is inside the range.
     * @param v: another positional instance.
     * @param range: an `angle` instance.
     */
    const bool
    neighbor_to(const vector3& v, const angle& range) const
    {
      const double cosine = std::cos(range.radian);
      const double sepcos = separation_cosine(v);
      return sepcos >= cosine;
    }

    /**
     * @brief return `true` if another point is inside the uncertainty.
     * @param v: another positional instance.
     */
    const bool
    neighbor_to(const vector3& v) const
    { return neighbor_to(v, s.radian); }

    /**
     * @brief return `true` if another point is inside the uncertainty.
     * @param p: another positional instance.
     * @note `range` is set the summation of the both uncertainties
     *       if `p` is an instance of `footprint`. as `range`.
     */
    const bool
    neighbor_to(const footprint& p) const
    { return neighbor_to(p, s.radian+p.s.radian); }

    /**
     * @brief obtain the footprint extended toward `q` by fraction `f`.
     * @param q: the anchor point.
     * @param f: the fraction of the length to `q`.
     * @note interpolation for `f` between [0,1], otherwise extrapolation.
     */
    const footprint
    extend_to(const footprint& q, const double f) const;
    // this method is defined later since it depends on `trail`.

    /**
     * @brief obtain the footprint extended toward `q` by separation `d`.
     * @param q: the anchor point.
     * @param d: the separation angle from this point.
     * @note interpolation for `f` between [0,1], otherwise extrapolation.
     */
    const footprint
    extend_to(const footprint& q, const angle& d) const;
    // this method is defined later.

    /**
     * @brief return `true` if another point is inside the range.
     * @param p: another positional instance.
     * @param range: an `angle` instance.
     * @param trange: a time duration.
     */
    const bool
    match(const footprint& p, const angle& range, const sec_t& trange) const
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
      const std::string& ss = timestamp_to_string(t);
      printf("%+.5lf %+.5lf %+.5lf %+.5lf %s\n",
             x, y, z, s.arcsec, ss.c_str());
    }

    const timestamp_t t; /** timestamp of the measurement. */
    const angle s;       /** uncertainty of the position. */

    /** `less` operator only takes the timestamp. */
    friend bool
    operator<(const footprint& lhs, const footprint& rhs)
    { return lhs.t < rhs.t; }
    /** `less-or-equal` operator only takes the timestamp. */
    friend bool
    operator<=(const footprint& lhs, const footprint& rhs)
    { return lhs.t <= rhs.t; }
    /** `greater` operator only takes the timestamp. */
    friend bool
    operator>(const footprint& lhs, const footprint& rhs)
    { return lhs.t > rhs.t; }
    /** `greater-or-equal` operator only takes the timestamp. */
    friend bool
    operator>=(const footprint& lhs, const footprint& rhs)
    { return lhs.t >= rhs.t; }

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
    const double
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
    { return _acos(separation_cosine(p)); }


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
    {
      for (const auto& v: list_around_pole(pole, N)) v.dump();
    }

    const direction_cosine pole; /** the pole of the great circle. */

  protected:
    /**
     * @brief dump (x,y,z)-coordinates on the circle.
     * @param _pole: the pole of the great circle.
     * @param N: the number of points.
     */
    const std::vector<direction_cosine>
    list_around_pole(const direction_cosine& _pole,
                     const direction_cosine& _anchor,
                     const size_t N) const
    {
      std::vector<direction_cosine> list; list.reserve(N);
      for (size_t i=0; i<N; i++) {
        const double phi = 2*M_PI/(N-1)*i;
        list.push_back(_anchor.pivot(_pole, phi, M_PI_2));
      }
      return list;
    }
    /**
     * @brief dump (x,y,z)-coordinates on the circle.
     * @param _pole: the pole of the great circle.
     * @param _anchor: the anchor point to make the circle.
     * @param N: the number of points.
     */
    const std::vector<direction_cosine>
    list_around_pole(const direction_cosine& _pole,
                     const size_t N) const
    {
      const direction_cosine p(1,0,0), q(0,1,0);
      const double&& dcosp = _pole.separation_cosine(p);
      const double&& dcosq = _pole.separation_cosine(q);
      const direction_cosine _anchor = _pole.outer_product(dcosq>dcosp?p:q);
      return list_around_pole(_pole, _anchor, N);
    }

    /**
     * @brief a helper function to calculate the distance to the arc.
     * @param s: the starting point of the arc.
     * @param e: the end point of the arc.
     * @param p: the distance from thie point is measured.
     * @note this function is defined for inherited arc classes.
     */
    const double
    distance_cosine(const direction_cosine& s,
                    const direction_cosine& e,
                    const direction_cosine& p) const
    {
      const direction_cosine ft = foot_of(p);
      const double&& cosd_ps = s.separation_cosine(ft);
      const double&& cosd_pe = e.separation_cosine(ft);
      const double&& cosd_se = s.separation_cosine(e);
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

  private:
  };


  class minor_arc : public great_circle {
  public:
    /** disable the constructor without an argument */
    minor_arc() = delete;

    /**
     * @brief construct a `minor_arc` instance from `s` to `e`.
     * @param _s: the starting point of the arc.
     * @param _e: the end pont of the arc.
     * @note throw an exception when
     *         (invalid_argumet): a pole cannot be defined by `s` and `e`.
     */
    minor_arc(const direction_cosine& _s, const direction_cosine& _e)
      : great_circle(_s.get_pole(_e)), s(_s), e(_e) {}

    /**
     * @brief calculate `cos(d)` between the arc and the point `p`.
     * @param p: a `direction_cosine` instance.
     */
    const double
    distance_cosine(const direction_cosine& p) const
    {
      return great_circle::distance_cosine(s,e,p);
    }

    /**
     * @brief calculate the distance between the arc and the point `p`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    distance(const direction_cosine& p) const
    {
      return _acos(distance_cosine(p));
    }

    /**
     * @brief return an extrapolated point of the arc.
     * @param f: fraction of the extrapolation. the end point of the arc
     *           is obtained for f = 1.
     */
    const direction_cosine
    extrapolate(const double f) const
    {
      return s.extend_to(e, f);
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
  };


  class trail : public great_circle {
  public:
    /** disable the constructor without arguments. */
    trail() = delete;

    /** copy constructor */
    trail(const trail& copy)
      : trail(copy.s, copy.e) {}

    /**
     * @brief construct a `minor_arc` instance from `s` to `e`.
     * @param _s: the starting point of the arc.
     * @param _e: the end pont of the arc.
     * @note throw an exception when
     *         (invalid_argumet): a pole cannot be defined by `s` and `e`.
     *         (invalid_argumet): timestamps of `s` and `e` are the same.
     */
    trail(const footprint& _s, const footprint& _e)
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
        printf("# trail::trail\n");
        printf("#   s   : "); s.dump();
        printf("#   e   : "); e.dump();
        printf("#   dt  : %+lf s\n", dt.count());
        printf("#   h_s1: "); h_s1.dump();
        printf("#   h_s2: "); h_s2.dump();
        printf("#   h_e1: "); h_e1.dump();
        printf("#   h_e2: "); h_e2.dump();
        printf("#   p_s1: "); p_s1.dump();
        printf("#   p_s2: "); p_s2.dump();
        printf("#   p_e1: "); p_e1.dump();
        printf("#   p_e2: "); p_e2.dump();
      }
    }

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
      return _acos(separation_cosine(gc));
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
      return _acos(separation_cosine(p));
    }

    /**
     * @brief calculate `cos(d)` between the arc and the point `p`.
     * @param p: a `direction_cosine` instance.
     */
    const double
    distance_cosine(const direction_cosine& p) const
    {
      return great_circle::distance_cosine(s,e,p);
    }

    /**
     * @brief calculate the distance between the arc and the point `p`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    distance(const direction_cosine& p) const
    {
      return _acos(distance_cosine(p));
    }

    /**
     * @brief return an extrapolated point of the arc.
     * @param f: fraction of the extrapolation. the end point of the arc
     *           is obtained for f = 1.
     */
    const direction_cosine
    extrapolate(const double f) const
    {
      return s.extend_to(e, f);
    }

    /**
     * @brief obtain the point after `dT` from `e`.
     * @param dT: duration in second.
     */
    const footprint
    propagate(const sec_t& dT) const
    {
      const double f = 1.0+(double)(dT/dt);
      const auto&& T = advance_timestamp(e.t, dT);
      const direction_cosine q = extrapolate(f);
      const angle&& qs = error_at(q);
      if (__debug__) {
        printf("# footprint::propagate\n");
        printf("#   f: %+lf\n", f);
        printf("#   q: "); q.dump();
        printf("#   s: %+lf\n", qs.arcsec);
      }
      return footprint(q.l,q.m,q.n,T,qs);
    }

    /**
     * @brief obtain the point at `T`.
     * @param T: timestamp instance.
     */
    const footprint
    propagate(const timestamp_t& T) const
    {
      return propagate(T - e.t);
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
     * @param arc: a `trail` instance.
     */
    const bool
    intersect_with(const trail& arc) const
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
      if (!intersect_with(arc)) return false;
      const double&& cosd = separation_cosine(arc);
      return cosd >= std::cos(tol.radian);
    }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `trail` instance.
     */
    const bool
    colinear_with(const trail& arc,
                  const angle& tol = degree(5.0)) const
    {
      if (tol.degree > 90.0)
        throw std::invalid_argument("invalid tolerance value.");
      if (!intersect_with(arc)) return false;
      const double&& cosd = separation_cosine(arc);
      return cosd >= std::cos(tol.radian);
    }


    /**
     * @brief check if the two trails are consistent or not.
     * @param arc: another arc.
     * @param dtol: a torelance in direction.
     * @param rtol: a torelance in range.
     * @param margin: an uncertainty multiplication factor.
     */
    const bool
    match(const trail& arc,
          const angle& dtol = degree(5.0),
          const angle& rtol = arcmin(5.0),
          const double margin = 1.0) const
    {
      if (!colinear_with(arc)) return false;
      const auto pred_s = propagate(arc.s.t);
      const auto pred_e = propagate(arc.e.t);
      const angle&& dist_s = pred_s.separation(arc.s);
      const angle&& dist_e = pred_e.separation(arc.e);
      if (__debug__) {
        printf("# footprint::match");
        printf("#   pred_s : %+g\n", (double)pred_s.s);
        printf("#          : %+g\n", (double)(pred_s.s*margin+rtol));
        printf("#   dist_s : %+g\n", (double)dist_s);
        printf("#   pred_e : %+g\n", (double)pred_e.s);
        printf("#          : %+g\n", (double)(pred_e.s*margin+rtol));
        printf("#   dist_e : %+g\n", (double)dist_e);
      }
      return (pred_s.s*margin+rtol>dist_s)&&(pred_e.s*margin+rtol>dist_e);
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
      return _acos(std::min({d_s1,d_s2,d_e1,d_e2}));
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
      printf("\n");
    }

    /**
     * @brief dump (x,y,z)-coordinates on the uncertainty circles.
     * @param N: the number of points (default: 64).
     */
    void dump_error(const size_t N=64) const
    {
      const auto plist = list_around_pole(pole, s, N);
      const auto gc_s1 = great_circle(p_s1);
      const auto gc_s2 = great_circle(p_s2);
      const auto gc_e1 = great_circle(p_e1);
      const auto gc_e2 = great_circle(p_e2);
      for (const auto p: plist) {
        const auto f_s1 = gc_s1.foot_of(p);
        const auto f_s2 = gc_s2.foot_of(p);
        const auto f_e1 = gc_e1.foot_of(p);
        const auto f_e2 = gc_e2.foot_of(p);
        const auto df_s1 = p.separation_cosine(f_s1);
        const auto df_s2 = p.separation_cosine(f_s2);
        const auto df_e1 = p.separation_cosine(f_e1);
        const auto df_e2 = p.separation_cosine(f_e2);
        const auto tmp_1 = (df_s1<df_e2)?f_s1:f_e2;
        const auto tmp_2 = (df_s2<df_e1)?f_s2:f_e1;
        const auto d1 = tmp_1.separation_cosine(pole);
        const auto d2 = tmp_2.separation_cosine(pole);
        (d1<d2)?tmp_1.dump():tmp_2.dump();
        (d1<d2)?tmp_2.dump():tmp_1.dump();
        printf("\n");
      }
    }

    const footprint s; /** the starting point of the arc. */
    const footprint e; /** the end point of the arc. */
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
    make_helper(const footprint& from, const footprint& to, const bool parity)
    {
      const angle theta = from.separation(to);
      const angle& delta = to.s;
      const angle&& b = theta.radian*theta.radian-delta.radian*delta.radian;
      const angle phi = radian(std::sqrt(b.radian));
      if (__debug__) {
        printf("# trail::make_helper\n");
        printf("#   from  : "); from.dump();
        printf("#   to    : "); to.dump();
        printf("#   parity: %s\n",(parity?"true":"false"));
        printf("#   theta : %+lf\n", theta.degree);
        printf("#   delta : %+lf\n", delta.degree);
        printf("#   phi   : %+lf\n", phi.degree);
      }
      if (delta > theta)
        throw std::invalid_argument("too large uncertainty.");
      return from.pivot(to, phi, (parity?1.0:-1.0)*delta);
    }
  };


  const footprint
  footprint::extend_to(const footprint& q, const double f) const
  {
    const timestamp_t eT = advance_timestamp(t, f*(q.t-t));
    const direction_cosine& ep = direction_cosine::extend_to(q,f);
    const double&& es = trail(*this, q).error_at(ep);
    return footprint(ep.l,ep.m,ep.n,eT,es);
  }

  const footprint
  footprint::extend_to(const footprint& q, const angle& d) const
  {
    return extend_to(q, d.radian/separation(q).radian);
  }


  class matrix3 {
  public:
    /**
     * @brief construct the identity matrix
     */
    matrix3()
      : arr({1,0,0,0,1,0,0,0,1})
    {}

    /**
     * @brief contruct a `matrix3` instance from an array.
     * @param _arr: an array contains 9 double elements.
     */
    matrix3(const std::array<double,9> _arr)
      : arr(_arr)
    {}

    /**
     * @brief access to the i-th element of the matrix.
     * @note The alignment of the elements is as follows:
     *
     *       A =  | A[0] A[1] A[2] |
     *            | A[3] A[4] A[5] |
     *            | A[6] A[7] A[8] |
     */
    const double
    operator[] (const size_t i) const
    { return arr[i]; }

    /**
     * @brief access to the (i,j)-th element of the matrix.
     * @note The alignment of the elements is as follows:
     *
     *       A =  | A(0,0) A(0,1) A(0,2) |
     *            | A(1,0) A(1,1) A(1,2) |
     *            | A(2,0) A(2,1) A(2,2) |
     */
    const double
    operator() (const size_t i, const size_t j) const
    { return arr[3*i+j]; }


    /**
     * @brief returns a transpose of the `matrix3` instance.
     */
    const matrix3
    T() const
    {
      return matrix3
        ({arr[0],arr[3],arr[6],arr[1],arr[4],arr[7],arr[2],arr[5],arr[8]});
    }

    /** `product` operator with `vector3` and `direction_cosine` */
    template<typename positional>
    const positional
    operator* (const positional& v) const
    {
      const double&& x = arr[0]*v.x+arr[1]*v.y+arr[2]*v.z;
      const double&& y = arr[3]*v.x+arr[4]*v.y+arr[5]*v.z;
      const double&& z = arr[6]*v.x+arr[7]*v.y+arr[8]*v.z;
      return positional(x,y,z);
    }
    /** `product` operator with `footprint`. */
    const footprint
    operator* (const footprint& p) const
    {
      const double&& x = arr[0]*p.x+arr[1]*p.y+arr[2]*p.z;
      const double&& y = arr[3]*p.x+arr[4]*p.y+arr[5]*p.z;
      const double&& z = arr[6]*p.x+arr[7]*p.y+arr[8]*p.z;
      return footprint(x,y,z,p.t,p.s);
    }

    /** `unary minus` operator. */
    const matrix3
    operator- () const
    {
      return matrix3({-arr[0],-arr[1],-arr[2],-arr[3],-arr[4],
            -arr[5],-arr[6],-arr[7],-arr[8]});
    }
    /** `add` operator with a `matrix3` instance. */
    const matrix3
    operator+ (const matrix3& m) const
    {
      return matrix3({arr[0]+m[0],arr[1]+m[1],arr[2]+m[2],
            arr[3]+m[3],arr[4]+m[4],arr[5]+m[5],arr[6]+m[6],
            arr[7]+m[7],arr[8]+m[8]});
    }
    /** `minus` operator with a `matrix3` instance. */
    const matrix3
    operator- (const matrix3& m) const
    {
      return matrix3({arr[0]-m[0],arr[1]-m[1],arr[2]-m[2],
            arr[3]-m[3],arr[4]-m[4],arr[5]-m[5],arr[6]-m[6],
            arr[7]-m[7],arr[8]-m[8]});
    }
    /** `product` operator with a `matrix3` instance. */
    const matrix3
    operator* (const matrix3& m) const
    {
      const double&& a00 = arr[0]*m.arr[0]+arr[1]*m.arr[3]+arr[2]*m.arr[6];
      const double&& a01 = arr[0]*m.arr[1]+arr[1]*m.arr[4]+arr[2]*m.arr[7];
      const double&& a02 = arr[0]*m.arr[2]+arr[1]*m.arr[5]+arr[2]*m.arr[8];
      const double&& a10 = arr[3]*m.arr[0]+arr[4]*m.arr[3]+arr[5]*m.arr[6];
      const double&& a11 = arr[3]*m.arr[1]+arr[4]*m.arr[4]+arr[5]*m.arr[7];
      const double&& a12 = arr[3]*m.arr[2]+arr[4]*m.arr[5]+arr[5]*m.arr[8];
      const double&& a20 = arr[6]*m.arr[0]+arr[7]*m.arr[3]+arr[8]*m.arr[6];
      const double&& a21 = arr[6]*m.arr[1]+arr[7]*m.arr[4]+arr[8]*m.arr[7];
      const double&& a22 = arr[6]*m.arr[2]+arr[7]*m.arr[5]+arr[8]*m.arr[8];
      return matrix3({a00,a01,a02,a10,a11,a12,a20,a21,a22});
    }

    /** `add` operation with a floting point value. */
    friend const matrix3
    operator+ (const matrix3& m, const double x)
    {
      return matrix3({m.arr[0]+x,m.arr[1]+x,m.arr[2]+x,
            m.arr[3]+x,m.arr[4]+x,m.arr[5]+x,
            m.arr[6]+x,m.arr[7]+x,m.arr[8]+x});
    }
    /** `add` operation with a floting point value. */
    friend const matrix3
    operator+ (const double x, const matrix3& m)
    {
      return matrix3({x+m.arr[0],x+m.arr[1],x+m.arr[2],
            x+m.arr[3],x+m.arr[4],x+m.arr[5],
            x+m.arr[6],x+m.arr[7],x+m.arr[8]});
    }
    /** `subtract` operation with a floting point value. */
    friend const matrix3
    operator- (const matrix3& m, const double x)
    {
      return matrix3({m.arr[0]-x,m.arr[1]-x,m.arr[2]-x,
            m.arr[3]-x,m.arr[4]-x,m.arr[5]-x,
            m.arr[6]-x,m.arr[7]-x,m.arr[8]-x});
    }
    /** `subtract` operation with a floting point value. */
    friend const matrix3
    operator- (const double x, const matrix3& m)
    {
      return matrix3({x-m.arr[0],x-m.arr[1],x-m.arr[2],
            x-m.arr[3],x-m.arr[4],x-m.arr[5],
            x-m.arr[6],x-m.arr[7],x-m.arr[8]});
    }
    /** `product` operation with a floting point value. */
    friend const matrix3
    operator* (const matrix3& m, const double x)
    {
      return matrix3({m.arr[0]*x,m.arr[1]*x,m.arr[2]*x,
            m.arr[3]*x,m.arr[4]*x,m.arr[5]*x,
            m.arr[6]*x,m.arr[7]*x,m.arr[8]*x});
    }
    /** `product` operation with a floting point value. */
    friend const matrix3
    operator* (const double x, const matrix3& m)
    {
      return matrix3({x*m.arr[0],x*m.arr[1],x*m.arr[2],
            x*m.arr[3],x*m.arr[4],x*m.arr[5],
            x*m.arr[6],x*m.arr[7],x*m.arr[8]});
    }
    /** `divide` operation with a floting point value. */
    friend const matrix3
    operator/ (const matrix3& m, const double x)
    {
      return matrix3({m.arr[0]/x,m.arr[1]/x,m.arr[2]/x,
            m.arr[3]/x,m.arr[4]/x,m.arr[5]/x,
            m.arr[6]/x,m.arr[7]/x,m.arr[8]/x});
    }

    /**
     * @brief dump matrix elements in the standard output.
     */
    void
    dump() const
    {
      printf("  | %+.2f %+.2f %+.2f |\n", arr[0],arr[1],arr[2]);
      printf("  | %+.2f %+.2f %+.2f |\n", arr[3],arr[4],arr[5]);
      printf("  | %+.2f %+.2f %+.2f |\n", arr[6],arr[7],arr[8]);
    }

  private:
    const std::array<double,9> arr; /** elements of `matrix3` */
  };

  /**
   * @brief construct an rotation matrix around the x-axis.
   * @param t: a rotation angle.
   */
  const matrix3
  rotation_matrix_x(const angle& t)
  { return matrix3
      ({1,0,0,0,std::cos(t),-std::sin(t),0,std::sin(t),std::cos(t)}); }
  /**
   * @brief construct an rotation matrix around the y-axis.
   * @param t: a rotation angle.
   */
  const matrix3
  rotation_matrix_y(const angle& t)
  { return matrix3
      ({std::cos(t),0,std::sin(t),0,1,0,-std::sin(t),0,std::cos(t)}); }
  /**
   * @brief construct an rotation matrix around the z-axis.
   * @param t: a rotation angle.
   */
  const matrix3
  rotation_matrix_z(const angle& t)
  { return matrix3
      ({std::cos(t),-std::sin(t),0,std::sin(t),std::cos(t),0,0,0,1}); }

  namespace matrix {
    const auto& Rx = rotation_matrix_x; /** alias to `rotation_matrix_x` */
    const auto& Ry = rotation_matrix_y; /** alias to `rotation_matrix_y` */
    const auto& Rz = rotation_matrix_z; /** alias to `rotation_matrix_z` */
  }

  /**
   * @brief solve `Ax=b` using the cholesky decomposition.
   * @param A: a positive definite `matrix3` instance.
   * @param b: a coefficient vector.
   * @note returns a wrong result if `A` is not positive definite.
   */
  const vector3
  solve_chol3(const matrix3& A, const vector3& b)
  {
    std::array<double,3> D;
    std::array<double,3> L;
    D[0] = A(0,0);
    L[0] = A(1,0)/D[0];
    D[1] = A(1,1)-L[0]*L[0]*D[0];
    L[1] = A(2,0)/D[0];
    L[2] = (A(2,1)-L[0]*L[1]*D[0])/D[1];
    D[2] = A(2,2)-L[1]*L[1]*D[0]-L[2]*L[2]*D[1];
    const double&  z0 = b.x;
    const double&& z1 = b.y -z0*L[0];
    const double&& z2 = b.z -z0*L[1] -z1*L[2];
    const double&& y0 = z0/D[0];
    const double&& y1 = z1/D[1];
    const double&& y2 = z2/D[2];
    const double&  x2 = y2;
    const double&& x1 = y1 -x2*L[2];
    const double&& x0 = y0 -x1*L[0] -x2*L[1];
    return {x0,x1,x2};
  }

  /**
   * @brief obtain the eigen vector using the power method.
   * @param A: a `matrix3` instance.
   * @param v0: an initial guess (optional).
   * @param N: the number of iterations.
   */
  const vector3
  eigen_pow(const matrix3& A,
            const vector3& v0 = {1,0,0},
            const size_t   N = 50)
  {
    std::unique_ptr<vector3> vp = std::make_unique<vector3>(A*v0);
    for (size_t i=0; i<N; i++) {
      const auto vm = A*(*vp);
      vp.reset(new vector3(vm.x/vm.d,vm.y/vm.d,vm.z/vm.d));
    }
    return *vp;
  }


  class trajectory {
  public:
    /** disable the constructor without an argument. */
    trajectory() = delete;

    /**
     * @brief construct a `trajectory` instance with a single trail.
     * @param t: a `trail` instance.
     */
    trajectory(const trail& t)
    {
      tracklets.push_back(t);
      update();
    };

    /**
     * @brief construc a `trajectory` instance with multiple trails.
     * @param list: a list of trails.
     */
    trajectory(const std::initializer_list<trail> list)
    {
      for (const auto& t: list) tracklets.push_back(t);
      update();
    }

    /**
     * @brief construc a `trajectory` instance with multiple trails.
     * @param list: a list of trails.
     */
    trajectory(const std::vector<trail> list)
    {
      for (const auto& t: list) tracklets.push_back(t);
      update();
    }

    /** reference to the current arc. */
    const trail& arc() const { return *ptr_arc; }
    /** reference to the current starting point. */
    const footprint& s() const { return *ptr_s; }
    /** reference to the current end point. */
    const footprint& e() const { return *ptr_e; }
    /** reference to the current trail list. */
    const std::vector<trail>& list() const { return tracklets; }

    /**
     * @brief uppend a trail and update the trajectory.
     * @param t: a `trail` instance.
     */
    void
    append(const trail& t)
    {
      tracklets.push_back(t);
      update();
    }

    /**
     * @brief uppend a trail and update the trajectory.
     * @param t: a list of trails.
     */
    void
    append(const std::initializer_list<trail> list)
    {
      for (const auto& t: list) tracklets.push_back(t);
      update();
    }

    /**
     * @brief uppend a trail and update the trajectory.
     * @param t: a list of trails.
     */
    void
    append(const std::vector<trail> list)
    {
      for (const auto& t: list) tracklets.push_back(t);
      update();
    }

    /**
     * @brief obtain `cos(d)` to another `great_circle`.
     * @param gc: a `great_circle` instance.
     */
    const double
    separation_cosine(const great_circle& gc) const
    { return ptr_arc->separation_cosine(gc); }

    /**
     * @brief obtain the separation angle to another `great_circle`.
     * @param gc: a `great_circle` instance.
     */
    const angle
    separation(const great_circle& gc) const
    { return ptr_arc->separation(gc); }

    /**
     * @brief obtain `cos(d)` to `direction_cosine`.
     * @param p: a `direction_cosine` instance.
     */
    const double
    separation_cosine(const direction_cosine& p) const
    { return ptr_arc->separation_cosine(p); }

    /**
     * @brief obtain the separation angle to `direction_cosine`.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation(const direction_cosine& p) const
    { return ptr_arc->separation(p); }

    /**
     * @brief calculate the distance to the arc.
     * @param s: the starting point of the arc.
     * @param e: the end point of the arc.
     * @param p: the distance from thie point is measured.
     */
    const double
    distance_cosine(const direction_cosine& p) const
    { return ptr_arc->distance_cosine(p); }

    /**
     * @brief calculate the distance to the arc.
     * @param s: the starting point of the arc.
     * @param e: the end point of the arc.
     * @param p: the distance from thie point is measured.
     */
    const angle
    distance(const direction_cosine& p) const
    { return ptr_arc->distance(p); }

    /**
     * @brief return an extrapolated point of the arc.
     * @param f: fraction of the extrapolation. the end point of the arc
     *           is obtained for f = 1.
     */
    const direction_cosine
    extrapolate(const double f) const
    { return ptr_s->extend_to(*ptr_e, f); }

    /**
     * @brief obtain the point after `dT` from `e`.
     * @param dT: duration since the end point.
     * @param R: a regularization term.
     */
    const footprint
    propagate(const sec_t& dT, const double R = 100.0) const
    {
      const sec_t dt = dT - ptr_arc->dt;
      const double f = 1.0+(double)(dT/ptr_arc->dt);
      const auto&& T = advance_timestamp(ptr_e->t, dT);
      const direction_cosine q = extrapolate(f+fac(T,R));
      const angle&& qs = error_at(q);
      if (__debug__) {
        printf("# trajectory::propagate\n");
        printf("#   f  : %+lf\n", f);
        printf("#   dT : %+lf\n", dT.count());
        printf("#   fac: %+lf\n", fac(T,R));
        printf("#   q  : "); q.dump();
        printf("#   s  : %+lf\n", qs.arcsec);
      }
      return footprint(q.l,q.m,q.n,T,qs);
    }

    /**
     * @brief obtain the point at `T`.
     * @param T: timestamp instance.
     */
    const footprint
    propagate(const timestamp_t& T) const
    { return propagate(T-ptr_e->t); }

    /**
     * @brief check if the arc intersects with the position `p` taking
     *        into account the uncertainties of the end points.
     * @param p: a `direction_cosine` instance.
     */
    const bool
    intersect_with(const direction_cosine& p) const
    { return ptr_arc->intersect_with(p); }

    /**
     * @brief check if the arc intersects with the arc `arc` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `minor_arc` instance.
     */
    const bool
    intersect_with(const minor_arc& arc) const
    { return ptr_arc->intersect_with(arc); }

    /**
     * @brief check if the arc intersects with the arc `arc` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `trail` instance.
     */
    const bool
    intersect_with(const trail& arc) const
    { return ptr_arc->intersect_with(arc); }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param gc: a `great_circle` instance.
     */
    const bool
    colinear_with(const great_circle& gc,
                  const angle& tol = degree(5.0)) const
    { return ptr_arc->colinear_with(gc, tol); }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `minor_arc` instance.
     */
    const bool
    colinear_with(const minor_arc& arc,
                  const angle& tol = degree(5.0)) const
    { return ptr_arc->colinear_with(arc, tol); }

    /**
     * @brief check if the arc is colinear with a `great_circle` taking
     *        into account the uncertainties of the end points.
     * @param arc: a `trail` instance.
     */
    const bool
    colinear_with(const trail& arc,
                  const angle& tol = degree(5.0)) const
    { return ptr_arc->colinear_with(arc, tol); }

    /**
     * @brief check if the two trails are consistent or not.
     * @param arc: another arc.
     * @param dtol: a torelance in direction.
     * @param rtol: a torelance in range.
     * @param margin: an uncertainty multiplication factor.
     */
    const bool
    match(const trail& arc,
          const angle& dtol = degree(5.0),
          const angle& rtol = arcmin(5.0),
          const double margin = 1.0) const
    {
      if (!colinear_with(arc)) return false;
      const auto pred_s = propagate(arc.s.t);
      const auto pred_e = propagate(arc.e.t);
      const angle&& dist_s = pred_s.separation(arc.s);
      const angle&& dist_e = pred_e.separation(arc.e);
      if (__debug__) {
        printf("# trajectory::match");
        printf("#   pred_s : %+g\n", (double)pred_s.s);
        printf("#          : %+g\n", (double)(pred_s.s*margin+rtol));
        printf("#   dist_s : %+g\n", (double)dist_s);
        printf("#   pred_e : %+g\n", (double)pred_e.s);
        printf("#          : %+g\n", (double)(pred_e.s*margin+rtol));
        printf("#   dist_e : %+g\n", (double)dist_e);
      }
      return (pred_s.s*margin+rtol>dist_s)&&(pred_e.s*margin+rtol>dist_e);
    }

    /**
     * @brief return the uncertainty at the foot of `q`.
     * @param q: a `direction_cosine` instance.
     * @param skip: skip execution of `foot_of` if true.
     */
    const angle
    error_at(const direction_cosine& q, const bool skip=true) const
    { return ptr_arc->error_at(q, skip); }

    /**
     * @brief dump (x,y,z)-coordinates of the great circle.
     * @param N: the number of points (default: 64).
     */
    void
    dump(const size_t N=64) const
    { ptr_arc->dump(N); }

    /**
     * @brief dump (x,y,z)-coordinates of the arc.
     * @param N: the number of points (default: 64).
     */
    void
    dump_arc(const size_t N=64) const
    { ptr_arc->dump_arc(N); }

    /**
     * @brief dump (x,y,z)-coordinates on the uncertainty circles.
     * @param N: the number of points (default: 64).
     */
    void
    dump_error(const size_t N=64) const
    { ptr_arc->dump_error(N); }

  private:
    /**
     * @brief update the trajectory instance.
     */
    void
    update()
    {
      double ll(0), lm(0), ln(0), mm(0), mn(0), nn(0);
      ptr_s.reset(new footprint(tracklets[0].s));
      ptr_e.reset(new footprint(tracklets[0].e));
      for (const auto& t: tracklets) {
        ll += t.pole.l*t.pole.l; lm += t.pole.l*t.pole.m;
        ln += t.pole.n*t.pole.l; mm += t.pole.m*t.pole.m;
        mn += t.pole.m*t.pole.n; nn += t.pole.n*t.pole.n;
        if (*ptr_s>t.s) ptr_s.reset(new footprint(t.s));
        if (*ptr_e<t.e) ptr_e.reset(new footprint(t.e));
      }
      const matrix3 A({ll,lm,ln,lm,mm,mn,ln,mn,nn});
      const vector3 pole(eigen_pow(A));
      const great_circle gc(pole);
      const direction_cosine tmps = gc.foot_of(*ptr_s);
      const direction_cosine tmpe = gc.foot_of(*ptr_e);
      ptr_s.reset(new footprint(tmps.l,tmps.m,tmps.n,ptr_s->t,ptr_s->s));
      ptr_e.reset(new footprint(tmpe.l,tmpe.m,tmpe.n,ptr_e->t,ptr_e->s));
      ptr_arc.reset(new trail(*ptr_s,*ptr_e));

      if (__debug__) {
        printf("# trajectory::update\n");
        printf("#   pole: "); ptr_arc->pole.dump();
        printf("#   s   : "); ptr_s->dump();
        printf("#   e   : "); ptr_e->dump();
      }
      acc_xd = 0.0; acc_xx = 0.0;
      for (const auto& t: tracklets) {
        const sec_t te = ptr_e->t - ptr_s->t;
        {
          const double dts = ((sec_t)(t.s.t - ptr_s->t)).count();
          const double dte = ((sec_t)(t.s.t - ptr_e->t)).count();
          const double di = deviation_term(t.s);
          acc_xd += di*(dts*dte); acc_xx += (dts*dte)*(dts*dte);
        }
        {
          const double dts = ((sec_t)(t.e.t - ptr_s->t)).count();
          const double dte = ((sec_t)(t.e.t - ptr_e->t)).count();
          const double di = deviation_term(t.e);
          acc_xd += di*(dts*dte); acc_xx += (dts*dte)*(dts*dte);
        }
      }
      if (__debug__) {
        printf("#   xd : %lf\n", acc_xd);
        printf("#   xx : %lf\n", acc_xx);
        printf("\n\n");
      }
    }

    /**
     * @brief obtain the separation angle to the foot of the given point.
     * @param p: a `direction_cosine` instance.
     */
    const angle
    separation_to_foot_of(const direction_cosine& p) const
    {
      const direction_cosine&& f = ptr_arc->foot_of(p);
      return ptr_s->separation(f);
    }

    /**
     * @brief obtain the separation angle to the extrapolated point.
     * @param T: a `timestamp_t` instance.
     */
    const angle
    separation_to_propagated(const timestamp_t& T) const
    {
      const footprint&& p = ptr_arc->propagate(T);
      return ptr_s->separation(p);
    }

    /**
     * @brief deviation from the linear propagation.
     * @param p: a `footprint` instance.
     * @note negative deviation indicates acceleration, while positive
     *       deviation indicates deceleration.
     */
    const double
    deviation_term(const footprint& p) const
    {
      const angle&& df = separation_to_foot_of(p);
      const angle&& dp = separation_to_propagated(p.t);
      if (__debug__) {
        printf("# trajectory::deviation_term\n");
        printf("#   df  : %lf\n", df.radian);
        printf("#   dp  : %lf\n", dp.radian);
        printf("#   diff: %lf\n", df.radian-dp.radian);
      }
      return df.radian-dp.radian;
    }

    /**
     * @brief return a modification term for a giveNtime.
     * @param dT: a duration since the starting point.
     * @param R: a regularization term.
     * @note the time origin is different from footprint::propagate().
     */
    const double
    acceleration_term(const timestamp_t& T, const double R) const
    {
      const sec_t& dTs = T - ptr_s->t;
      const sec_t& dTe = T - ptr_e->t;
      return acc_xd*dTs.count()*dTe.count()/(acc_xx+R);
    }

    /**
     * @brief return a modification term for a giveNtime.
     * @param T: a `timestamp_t` instance.
     * @param R: a regularization term.
     */
    const double
    fac(const timestamp_t& T, const double R = 100.0) const
    {
      const sec_t& dT = ptr_arc->dt;
      return acceleration_term(T, R)/dT.count();
    }

    /**
     * @brief check if the two trails are consistent or not.
     * @param arc: another arc.
     * @param dtol: a torelance in direction.
     * @param rtol: a torelance in range.
     * @param margin: an uncertainty multiplication factor.
     * @note acceleration/deceleration are not taken into account.
     */
    const bool
    simple_match(const trail& arc,
                 const angle& dtol = degree(5.0),
                 const angle& rtol = arcmin(5.0),
                 const double margin = 1.0) const
    { return ptr_arc->match(arc, dtol, rtol, margin); }


    std::unique_ptr<trail> ptr_arc;   /** the trajectory arc. */
    std::unique_ptr<footprint> ptr_s; /** the starting point of the arc. */
    std::unique_ptr<footprint> ptr_e; /** the end point of the arc. */
    std::vector<trail> tracklets;     /** tracklets of trajectory. */
    double acc_xd = 0.0;              /** acceleration coefficient xd. */
    double acc_xx = 0.0;              /** acceleration coefficient xx. */
  };
}

#endif  // __GCXMLIB_H_INCLUDE
