# Constants and Aliases

The namespace `gcxmlib` contains following several constants and aliases. This section describes the definitions and general usages.


## Constants related to angles

The following constants are used to convert angles in different scales.

``` cpp
constexpr double radian_to_degree = 180./M_PI;
constexpr double degree_to_radian = M_PI/180.;
constexpr double radian_to_arcmin = 60.*180./M_PI;
constexpr double arcmin_to_radian = M_PI/180./60.;
constexpr double radian_to_arcsec = 3600.*180./M_PI;
constexpr double arcsec_to_radian = M_PI/180./3600.;
```

By using `namespace gcxmlib::literals`, you can use the following user-defined literals to define angles.

``` cpp
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
```

An example is provided below. An `angle` instance is defined in a user-friendly style using literals, instead of `degree(1.0)`.

``` cpp
using namespace gcxmlib::literals;
const angle one_degree = 1.0deg;
```


## Constants related to time

The `gcxmlib` depends on `std::chrono`. Aliases of frequently used names are provided as follows. Note that the `duration` of `default_clock` is different from `sec_t`. Thus, calculations between `timestamp_t` and `sec_t` is not defined. To advance/roll back the timestamp instance, use [`advance_timestamp()`](../functions/#advance_timestamp).

``` cpp
using sec_t = std::chrono::duration<double>;
using default_clock = std::chrono::system_clock;
using timestamp_t = std::chrono::time_point<default_clock>;
```


## Miscellaneous constants

The constants below are defined for the internal use. If `__debug__` is set `true`, messages for debugging are enabled.

``` cpp
constexpr bool __debug__ = false;
constexpr double __epsilon__ = 1e-12;
constexpr double __exact_zero__ = 0.0;
```
