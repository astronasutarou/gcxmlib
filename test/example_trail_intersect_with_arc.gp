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
data="< ./example_trail_intersect_with_arc"
set table $arc
plot data i 4 u 1:2:3:4:5 with table
unset table

splot data i 0 u 1:2:3 w l lc 1 t sprintf("great circle #_%d",1), \
      data i 1 u 1:2:3 w pm3d not, \
      data i 2 u 1:2:3 w pm3d not, \
      data i 3 u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      for [n=0:300] $arc u 3:4:($1==0?$5:1/0) \
      ev ::(2*n):(2*n+1) w lp pt 6 lc 3 not, \
      for [n=0:300] $arc u 3:4:($1==0?$5:1/0) \
      ev ::(2*n)::(2*n+1) w lp pt 6 lc 3 not, \
      for [n=0:300] $arc u 3:4:($1==1?$5:1/0) \
      ev ::(2*n)::(2*n+1) w lp pt 5 lc 7 not

pause 3
