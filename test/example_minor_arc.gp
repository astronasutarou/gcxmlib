#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_minor_arc"

splot for [n=0:1] data i 2*n u 1:2:3 w l lc n+1 \
      t sprintf("great circle #_%d",n+1),\
      for [n=0:1] data i 2*n+1 u 1:2:3 w l lw 3 lc n+7 \
      t sprintf("minor arc #_%d",n+1)
