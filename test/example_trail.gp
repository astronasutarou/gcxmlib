#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
set cbr [-1:1]
unset colorbox
set palette gray
set pm3d implicit at s noborder
set style fill transparent solid 0.4
data="< ./example_trail"
r='($1**2+$2**2+$3**2)'

splot for [n=0:1] data i 3*n u 1:2:3 w l lc n+1 \
      t sprintf("great circle #_%d",n+1),\
      for [n=0:1] data i 3*n+1 u 1:2:3:(0) w pm3d not, \
      for [n=0:1] data i 3*n+2 u 1:2:3 w l lw 6 lc n+7 \
      t sprintf("trail #_%d",n+1)
