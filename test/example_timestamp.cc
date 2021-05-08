/**
 * @file example_timestamp.cc
 * @brief functional test of the generate_time() function
 * @author Ryou Ohsawa
 * @year 2021
 */
#include "gcxmlib.h"
#include <random>

using gcxmlib::default_clock;
using gcxmlib::timestamp_t;
using gcxmlib::generate_timestamp;
using gcxmlib::timestamp_to_string;
using gcxmlib::sec_t;
using gcxmlib::advance_timestamp;

int
main(int argn, char** argv)
{
  int32_t seed = 42;
  std::mt19937 gen; gen.seed(seed);
  std::uniform_int_distribution<int32_t>
    year(1900,2100), month(1,12), day(1,31),
    hour(0,23), minute(0,59), second(0,59), usec(0,99999);
  auto s = std::chrono::system_clock::now();

  {
    const auto t0 = default_clock::now();
    printf("# current time in UTC:\n  %s\n",
           timestamp_to_string(t0).c_str());
  }

  printf("\n");

  {
    const auto t0 = generate_timestamp(1990,1,1,0,0,0,0);
    printf("# generate time (1990,1,1,0,0,0,0):\n  %s\n",
           timestamp_to_string(t0).c_str());
  }

  printf("\n");

  for (size_t i=0; i<10; i++) {
    const int32_t y = year(gen), m = month(gen), d = day(gen);
    const int32_t H = hour(gen), M = minute(gen), S = second(gen);
    const int32_t uS = usec(gen);
    const auto t0 = generate_timestamp(y,m,d,H,M,S,uS);
    printf("# generate time (%d,%d,%d,%d,%d,%d,%d):\n  %s\n",
           y,m,d,H,M,S,uS,timestamp_to_string(t0).c_str());
  }

  auto e = std::chrono::system_clock::now();
  std::chrono::duration<double> dt = e-s;
  printf("\n# elapsed time:: %.8lf ms\n", dt.count()*1e3);

  return 0;
}
