/**
 * @file sample_longitude.cc
 * @brief functional test of the longitude class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "spchord.h"
#include <random>

using spchord::longitude;


template<class T>
inline void
display_angle(const T angle)
{
  printf("[radian: %.2lf, degree: %.2lf, arcmin: %.2lf, arcsec: %.2lf]\n",
         angle.radian, angle.degree, angle.arcmin, angle.arcsec);
}

inline void
put_boolean(const bool b)
{
  if (b==true)
    printf("true\n");
  else
    printf("false\n");
}


int
main(int argn, char** argv)
{
  printf("The domain of the `longitude` instance is [0, 2pi).\n");
  printf("Create `longitude` instances with various angles.\n");
  printf("# longitude(1.0)\n  ");
  display_angle(longitude(1.0));
  printf("# longitude(1.5)\n  ");
  display_angle(longitude(1.5));

  printf("\n");
  printf("Angle is automatically wrapped within [0, 2pi).\n");
  printf("# longitude(10.0)\n  ");
  display_angle(longitude(10.0));
  printf("# longitude(-3.0)\n  ");
  display_angle(longitude(-3.0));
  printf("# longitude(2M_PI)\n  ");
  display_angle(longitude(2*M_PI));

  printf("\n");
  printf("Arithmetic operations with the `longitude` and float values.\n");
  printf("# 1.5 + longitude(1.0)\n  ");
  { auto x = 1.5 + longitude(1.0); display_angle(x); }
  printf("# longitude(1.0) + 0.5\n  ");
  { auto x = longitude(1.0) + 0.5; display_angle(x); }
  printf("# 1.5 - longitude(1.0)\n  ");
  { auto x = 1.5 - longitude(1.0); display_angle(x); }
  printf("# longitude(1.0) - 0.5\n  ");
  { auto x = longitude(1.0) - 0.5; display_angle(x); }
  printf("# 1.5 * longitude(1.0)\n  ");
  { auto x = 1.5 * longitude(1.0); display_angle(x); }
  printf("# longitude(1.0) * 0.5\n  ");
  { auto x = longitude(1.0) * 0.5; display_angle(x); }
  printf("# 1.5 / longitude(1.0)\n  ");
  { auto x = 1.5 / longitude(1.0); display_angle(x); }
  printf("# longitude(1.0) / 0.5\n  ");
  { auto x = longitude(1.0) / 0.5; display_angle(x); }

  printf("\n");
  printf("Arithmetic operations between the `longitude` instances.\n");
  printf("# longitude(1.0) + longitude(1.5)\n  ");
  { auto x = longitude(1.0) + longitude(1.5); display_angle(x); }
  printf("# longitude(1.0) - longitude(0.5)\n  ");
  { auto x = longitude(1.0) - longitude(0.5); display_angle(x); }

  printf("\n");
  printf("Compare two `longitude` instances.\n");
  printf("# longitude(2.0) < longitude(1.0)\n  ");
  put_boolean(longitude(2.0) < longitude(1.0));
  printf("# longitude(2.0) > longitude(1.0)\n  ");
  put_boolean(longitude(2.0) > longitude(1.0));
  printf("# longitude(2.0) == longitude(2.0)\n  ");
  put_boolean(longitude(2.0) == longitude(2.0));

  return 0;
}
