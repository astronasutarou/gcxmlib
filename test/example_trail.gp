#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_trail"

splot for [n=0:1] data i 3*n u 1:2:3 w l lc n+1 \
      t sprintf("great circle #_%d",n+1),\
      for [n=0:1] data i 3*n+1 u 1:2:3 w l lw 6 lc n+7 \
      t sprintf("motion arc #_%d",n+1), \
      for [m=0:3] for [n=0:1] data i 3*n+2 ev :::m::m \
      u 1:2:3 w l lw 1 lc n+1 not

pause -1
