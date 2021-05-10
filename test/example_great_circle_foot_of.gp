#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_great_circle_foot_of"

splot for [n=0:1] data i 2*n u 1:2:3 w l lc n+1 \
      t sprintf("great circle #_%d",n+1), \
      for [n=0:1] data i 2*n+1 u 1:2:3:4:5:6 w vec lc n+1 not
