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
data="< ./example_trail_intersect_with_point"

splot data i 0 u 1:2:3 w l lc 1 t sprintf("trail #_%d",1), \
      data i 1 u 1:2:3 w pm3d not, \
      data i 2 u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      data i 3 u 2:3:($1==0?$4:1/0) w p pt 6 ps 0.2 lc 3 t "not matched", \
      data i 3 u 2:3:($1==1?$4:1/0) w p pt 6 ps 0.5 lc 2 t "matched"
