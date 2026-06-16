# Functions

### Functions associated with Timestamp

#### const timestamp_t<br>now()

Obtain a `timestamp_t` instance of the current time.

#### const timestamp_t<br>generate_timestamp(const int32_t year, const int32_t month, const int32_t day, const int32_t hour, const int32_t minute, const int32_t second, const int32_t microsecond)

Obtain a `timestamp_t` instance of the given date and time with microsecond resolution. Note that the timezone settings are ignored; The timezone settings are reverted at the beginning and restored at the end.

year
: calendar year.

month
: calendar month.

day
: calendar day.

hour
: hour.

minute
: minute.

second
: second.

microsecond
: microsecond.

#### const std::string<br>timestamp_to_string(const timestamp_t& ts)

Convert the `timestamp_t` instance into `std::string`. Note that this function may returns a wrong string for the timestamp before 1990-01-01T00:00:00.

ts
: the `timestamp_t` instance.

#### template&lt;typename duration_t&gt;<br>const timestamp_t<br>advance_timestamp(const timestamp_t& t0, const duration_t& dt)

Calculate the `add` or `subtract` operations between a `timestamp_t` instance and a `duration` instance.

t0
: the origin `timestamp_t` instance.

dt
: the `duration` instance.


### Functions associated with Angle

#### const angle<br>radian(const double ang)

Convert a floating-point value into an `angle` instance in radian.

ang
: an angle in radian.

#### const angle<br>degree(const double ang)

Convert a floating-point value into an `angle` instance in degree.

ang
: an angle in degree.

#### const angle<br>arcmin(const double ang)

Convert a floating-point value into an `angle` instance in arcminute.

ang
: an angle in arcminute.

#### const angle<br>arcsec(const double ang)

Convert a floating-point value into an `angle` instance in arcsecond.

ang
: an angle in arcsecond.


### Functions associated with Matrix

#### const matrix3<br>rotation_matrix_x(const angle& t)

Generate a rotation matrix around the x-axis.

t
: the `angle` instance which determines a rotation angle.

#### const matrix3<br>rotation_matrix_y(const angle& t)

Generate a rotation matrix around the y-axis.

t
: the `angle` instance which determines a rotation angle.

#### const matrix3<br>rotation_matrix_z(const angle& t)

Generate a rotation matrix around the z-axis.

t
: the `angle` instance which determines a rotation angle.


Aliases to the rotation matrix functions are defined in the namespace `gcxmlib::matrix`.

``` cpp
namespace matrix {
  const auto& Rx = rotation_matrix_x; /** alias to `rotation_matrix_x` */
  const auto& Ry = rotation_matrix_y; /** alias to `rotation_matrix_y` */
  const auto& Rz = rotation_matrix_z; /** alias to `rotation_matrix_z` */
}
```

#### const vector3<br>solve_chol3(const matrix3& A, const vector3& b)

Solve a equation $Ax = b$ in terms of $x$ using the Cholesky decomposition. The coefficient matrix $A$ should be positive definite. Otherwise, this function returns a wrong result without any warnings.

A
: a `matrix3` instance; a coefficient matrix of the equation.

b
: a `vector3` instance.

#### const vector3<br>eigen_pow(const matrix3& A, const vector3& v0 = {1,0,0}, const size_t N = 50)

Obtain the eigenvector of a 3&times;3 matrix $A$ using the power method. Only the eigenvector for the maximum eigenvalue is returned. Note that the convergence of the calculation is not checked.

A
: a `matrix3` instance.

v0
: the initial guess of the eigenvector (optional).

N
: the number of iterations (optional). The default value is 50.
