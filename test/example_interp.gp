#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_interp"

splot for [n=0:1] data i 3*n u 1:2:3 w l lc n+1 \
      t sprintf("great circle #_%d",n+1), \
      for [n=0:1] data i 3*n+1 u 1:2:3 w lp ps 2 pt 4+2*n lc n+1 not, \
      for [n=0:1] data i 3*n+2 u 1:2:3 w lp ps 2 pt 5+2*n lc n+1 not

pause -1
