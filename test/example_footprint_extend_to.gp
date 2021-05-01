#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_footprint_extend_to"
asec2z(t) = atan(t/180./3600.*pi)
getsec(n) = substr(strcol(n),18,99)

splot data i 0 u 1:2:3 w l lc 1 t sprintf("great circle #_%d",1), \
      data i 1 u 1:2:3 w l lw 6 lc 7 t sprintf("motion arc #_%d",1), \
      for [n=0:3] data i 2 ev :::n::n u 1:2:3 w l lc 1 lw 0.5 not, \
      data i 3 u 1:2:3 w lp ps 1 pt 7 lc 2 not,\
      data i 3 u 1:2:3:(getsec(5)) w labels font ",10" \
      left rotate by 90 offset 0,2 not,\
      data i 3 u 1:2:($3+asec2z($4)) w l lc 3 lw 3 t "uncertainty",\
      data i 3 u 1:2:($3-asec2z($4)) w l lc 3 lw 3 not

pause 3
