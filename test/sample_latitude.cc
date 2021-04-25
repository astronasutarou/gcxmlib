/**
 * @file sample_latitude.cc
 * @brief functional test of the latitude class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "spchord.h"
#include <random>

using spchord::latitude;


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
  printf("The domain of the `latitude` instance is [-pi/2, pi/2).\n");
  printf("Create `latitude` instances with various angles.\n");
  printf("# latitude(1.0)\n  ");
  display_angle(latitude(1.0));
  printf("# latitude(1.5)\n  ");
  display_angle(latitude(1.5));
  printf("# latitude(-1.5)\n  ");
  display_angle(latitude(-1.5));

  printf("\n");
  printf("Throw an exception when the angle exceeds [-pi/2, pi/2].\n");
  try {
    printf("# latitude(10.0) -> ");
    display_angle(latitude(10.0));
  } catch (const std::exception& e) {
    printf("%s\n", e.what());
  }
  try {
    printf("# latitude(-5.0) -> ");
    display_angle(latitude(-5.0));
  } catch (const std::exception& e) {
    printf("%s\n", e.what());
  }
  try {
    printf("# latitude(M_PI_2+1e-15) -> ");
    display_angle(latitude(M_PI_2+1e-15));
  } catch (const std::exception& e) {
    printf("%s\n", e.what());
  }

  printf("\n");
  printf("Arithmetic operations with the `latitude` and float values.\n");
  printf("# 0.5 + latitude(1.0)\n  ");
  { auto x = 0.5 + latitude(1.0); display_angle(x); }
  printf("# latitude(1.0) + 0.1\n  ");
  { auto x = latitude(1.0) + 0.1; display_angle(x); }
  printf("# 1.5 - latitude(1.0)\n  ");
  { auto x = 1.5 - latitude(1.0); display_angle(x); }
  printf("# latitude(1.0) - 0.5\n  ");
  { auto x = latitude(1.0) - 0.5; display_angle(x); }
  printf("# 0.8 * latitude(1.0)\n  ");
  { auto x = 0.8 * latitude(1.0); display_angle(x); }
  printf("# latitude(1.0) * 0.2\n  ");
  { auto x = latitude(1.0) * 0.2; display_angle(x); }
  printf("# 1.5 / latitude(1.0)\n  ");
  { auto x = 1.5 / latitude(1.0); display_angle(x); }
  printf("# latitude(1.0) / 2.5\n  ");
  { auto x = latitude(1.0) / 2.5; display_angle(x); }

  printf("\n");
  printf("Arithmetic operations between the `latitude` instances.\n");
  printf("# latitude(1.0) + latitude(0.2)\n  ");
  { auto x = latitude(1.0) + latitude(0.2); display_angle(x); }
  printf("# latitude(1.0) - latitude(1.5)\n  ");
  { auto x = latitude(1.0) - latitude(1.5); display_angle(x); }

  printf("\n");
  printf("Compare two `latitude` instances.\n");
  printf("# latitude(2.0) < latitude(1.0)\n  ");
  put_boolean(latitude(1.0) < latitude(0.5));
  printf("# latitude(2.0) > latitude(1.0)\n  ");
  put_boolean(latitude(1.0) > latitude(0.5));
  printf("# latitude(2.0) == latitude(2.0)\n  ");
  put_boolean(latitude(1.0) == latitude(1.0));

  return 0;
}
