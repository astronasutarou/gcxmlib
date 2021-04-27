#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-2.5:2.5]
set yr [-2.5:2.5]
set zr [-2.5:2.5]

splot for [n=0:5] '< ./example_outer_product' \
      i n u (0):(0):(0):1:2:3 w vec lc n+1 \
      t sprintf("outer product #_%d",n+1)

pause -1
