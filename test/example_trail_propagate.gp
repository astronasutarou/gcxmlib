#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
set style fill transparent solid 0.2
data="< ./example_trail_propagate"
asec2z(t) = atan(t/180./3600.*pi)
getsec(n) = substr(strcol(n),18,99)

splot data i 0 u 1:2:3 w l lc 1 t "great circle #_1", \
      data i 1 u 1:2:3 w l lw 3 lc 7 t "trail #_1", \
      data i 3 u 1:2:3 w p pt 6 ps 1 lc 2 t "propagated #_1", \
      data i 3 u 1:2:3:(asec2z($4)) w zerror lc 2 not, \
      data i 3 ev 2 u 1:2:3:(getsec(5)) w labels font ",10" \
      left rotate by 90 offset 0,2 not,\
      data i 2 u 1:2:3:($0+6) w p pt var ps 1 lc 1 not, \
      data i 4 u 1:2:3 w l lc 1 t "great circle #_2", \
      data i 5 u 1:2:3 w l lw 3 lc 6 t "trail #_2", \
      data i 7 u 1:2:3 w p pt 6 ps 1 lc 4 t "propagated #_2", \
      data i 7 u 1:2:3:(asec2z($4)) w zerror lc 4 not, \
      data i 7 ev 2 u 1:2:3:(getsec(5)) w labels font ",10" \
      left rotate by 90 offset 0,2 not,\
      data i 6 u 1:2:3:($0+6) w p pt var ps 1 lc 1 not

pause 3
