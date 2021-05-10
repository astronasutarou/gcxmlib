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
data="< ./example_footprint_extend_to"
asec2z(t) = atan(t/180./3600.*pi)
getsec(n) = substr(strcol(n),18,99)

splot data i 0 u 1:2:3 w l lc 1 t sprintf("great circle #_%d",1), \
      data i 1 u 1:2:3:(0) w pm3d not, \
      data i 2 u 1:2:3 w l lw 3 lc 7 t sprintf("trail #_%d",1), \
      data i 3 u 1:2:3 w p ps 1 pt 7 lc 2 t "extended #_1",\
      data i 3 u 1:2:3:(getsec(5)) w labels font ",10" \
      left rotate by 90 offset 0,2 not
