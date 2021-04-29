#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_motion_arc"

splot for [n=0:1] data i n u 1:2:3 w l lc n+1 \
      t sprintf("motion arc #_%d",n+1)

pause -1
