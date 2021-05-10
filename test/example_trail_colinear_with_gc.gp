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
data="< ./example_trail_colinear_with_gc"
set table $circle
plot data i 3 w table
unset table

splot data i 0 u 1:2:3 w l lc 1 t sprintf("great circle #_%d",1), \
      data i 1 u 1:2:3:(0) w pm3d not, \
      data i 2 u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      $circle ev ::(0)::(63) u 1:2:3 \
      w lp ps 0.5 pt 2 lc 3 t "matched circle", \
      for [n=0:100] $circle ev ::(64*n)::(64*n+63) u 1:2:3 \
      w lp ps 0.2 lc 3 not
