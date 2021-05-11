#!/usr/bin/env gnuplot
set terminal wxt size 1000,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
set key at screen .98,.98
data="< ./example_trajectory_propagate"

splot data i 0 u 1:2:3 w l lc 1 dt 2 t "great circle #_1", \
      data i 1 u 1:2:3 w l lc 2 lw 3 t "trail arc #_1", \
      for [i=1:5] data i 1+i u 1:2:3 w l lc 3 lw 3 \
      t sprintf("new trail arc #_{1(%d)}",i), \
      data i 7 u 1:2:3 w p pt 6 lc 4 t "trajectory_1", \
      data i 8 u 1:2:3 w p pt 4 lc 7 lw 2 t "propagated_1", \
      data i 8 u 1:2:3:0 w labels offset 0,1 not, \
      data i 9 u 1:2:3 w l lc 1 dt 2 t "great circle #_2", \
      data i 10 u 1:2:3 w l lc 2 lw 3 t "trail arc #_2", \
      for [i=1:5] data i 10+i u 1:2:3 w l lc 3 lw 3 \
      t sprintf("new trail arc #_{2(%d)}",i), \
      data i 16 u 1:2:3 w p pt 6 lc 4 t "trajectory_2", \
      data i 17 u 1:2:3 w p pt 4 lc 7 lw 2 t "propagated_2 w/acc", \
      data i 17 u 1:2:3:0 w labels offset 0,1 not, \
      data i 18 u 1:2:3 w p pt 8 lc 7 lw 2 t "propagated_2 w/o acc", \
      data i 18 u 1:2:3:0 w labels offset 0,1 tc rgb "gray" not
