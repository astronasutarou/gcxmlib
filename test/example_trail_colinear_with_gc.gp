#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_trail_colinear_with_gc"

splot data i 0 u 1:2:3 w l lc 1 t sprintf("motion arc #_%d",1), \
      for [n=0:3] data i 1 u 1:2:3 ev :::n::n w l lc rgb "gray60" not, \
      data i 2 u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      data i 3 u 1:2:3 w p pt 6 lc 3 not

pause 3
