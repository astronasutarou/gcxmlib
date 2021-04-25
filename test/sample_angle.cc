/**
 * @file sample_angles.cc
 * @brief functional test of the angle class
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "spchord.h"
#include <random>

using spchord::angle;


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
  printf("The domain of the `angle` instance is [0, 2pi).\n");
  printf("Create `angle` instances with various angles.\n");
  printf("# angle(1.0)\n  ");
  display_angle(angle(1.0));
  printf("# angle(1.5)\n  ");
  display_angle(angle(1.5));

  printf("\n");
  printf("Angle is automatically wrapped within [0, 2pi).\n");
  printf("# angle(10.0)\n  ");
  display_angle(angle(10.0));
  printf("# angle(-3.0)\n  ");
  display_angle(angle(-3.0));
  printf("# angle(2M_PI)\n  ");
  display_angle(angle(2*M_PI));

  printf("\n");
  printf("Arithmetic operations with the `angle` and float values.\n");
  printf("# 1.5 + angle(1.0)\n  ");
  { auto x = 1.5 + angle(1.0); display_angle(x); }
  printf("# angle(1.0) + 0.5\n  ");
  { auto x = angle(1.0) + 0.5; display_angle(x); }
  printf("# 1.5 - angle(1.0)\n  ");
  { auto x = 1.5 - angle(1.0); display_angle(x); }
  printf("# angle(1.0) - 0.5\n  ");
  { auto x = angle(1.0) - 0.5; display_angle(x); }
  printf("# 1.5 * angle(1.0)\n  ");
  { auto x = 1.5 * angle(1.0); display_angle(x); }
  printf("# angle(1.0) * 0.5\n  ");
  { auto x = angle(1.0) * 0.5; display_angle(x); }
  printf("# 1.5 / angle(1.0)\n  ");
  { auto x = 1.5 / angle(1.0); display_angle(x); }
  printf("# angle(1.0) / 0.5\n  ");
  { auto x = angle(1.0) / 0.5; display_angle(x); }

  printf("\n");
  printf("Arithmetic operations between the `angle` instances.\n");
  printf("# angle(1.0) + angle(1.5)\n  ");
  { auto x = angle(1.0) + angle(1.5); display_angle(x); }
  printf("# angle(1.0) - angle(0.5)\n  ");
  { auto x = angle(1.0) - angle(0.5); display_angle(x); }

  printf("\n");
  printf("Compare two `angle` instances.\n");
  printf("# angle(2.0) < angle(1.0)\n  ");
  put_boolean(angle(2.0) < angle(1.0));
  printf("# angle(2.0) > angle(1.0)\n  ");
  put_boolean(angle(2.0) > angle(1.0));
  printf("# angle(2.0) == angle(2.0)\n  ");
  put_boolean(angle(2.0) == angle(2.0));

  return 0;
}
