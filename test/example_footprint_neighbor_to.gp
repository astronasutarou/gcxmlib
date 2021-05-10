#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-.3:.3]
set yr [-.3:.3]
set zr [0.505:1.05]
data="< ./example_footprint_neighbor_to"

splot data i 0 u 2:3:4 w d lc 1 not, \
      data i 0 u 2:3:($1>0?$4:-1) w p pt 6 lc 2 t "nearby footprints"
