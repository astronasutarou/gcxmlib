# Functions

#### const timestamp_t<br>now()
#### const timestamp_t<br>generate_timestamp(const int32_t year, const int32_t month, const int32_t day, const int32_t hour, const int32_t minute, const int32_t second, const int32_t microsecond)
#### const std::string<br>timestamp_to_string(const timestamp_t& ts)
#### template&lt;typename duration_t&gt;<br>const timestamp_t<br>advance_timestamp(const timestamp_t& t0, const duration_t& dt)

#### const angle<br>radian(const double ang)
#### const angle<br>degree(const double ang)
#### const angle<br>arcmin(const double ang)
#### const angle<br>arcsec(const double ang)


#### const matrix3<br>rotation_matrix_x(const angle& t)
#### const matrix3<br>rotation_matrix_y(const angle& t)
#### const matrix3<br>rotation_matrix_z(const angle& t)

``` c++
namespace matrix {
  const auto& Rx = rotation_matrix_x; /** alias to `rotation_matrix_x` */
  const auto& Ry = rotation_matrix_y; /** alias to `rotation_matrix_y` */
  const auto& Rz = rotation_matrix_z; /** alias to `rotation_matrix_z` */
}
```

#### const vector3<br>solve_chol3(const matrix3& A, const vector3& b)
#### const vector3<br>eigen_pow(const matrix3& A, const vector3& v0 = {1,0,0}, const size_t   N = 50)
