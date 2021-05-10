#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
unset colorbox
set palette gray
set pm3d implicit at s noborder
set style fill transparent solid .2
data="< ./example_trail_colinear_with_arc"
set table $arc
plot data i 4 w table
unset table

splot data i 0 u 1:2:3 w l lc 1 t sprintf("trail #_%d",1), \
      data i 1 u 1:2:3 w pm3d not, \
      data i 2 u 1:2:3 w pm3d not, \
      data i 3 u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      $arc ev ::(0)::(1) u 1:2:3:(6+int($0)%2) \
      w lp pt var lc 3 t "matched arc", \
      for [n=1:100] $arc ev ::(2*n)::(2*n+1) u 1:2:3:(6+int($0)%2) \
      w lp pt var lc 3 not
