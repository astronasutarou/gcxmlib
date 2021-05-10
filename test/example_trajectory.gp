#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_trajectory"

splot data i 0 u 1:2:3 w l lc 1 dt 2 t "original circle", \
      data i 1 u 1:2:3 w l lc 2 lw 3 t "original trail arc", \
      data i 2 u 1:2:3 w l lc 1 dt 2 t "great circle #_1", \
      data i 3 u 1:2:3 w l lc 2 lw 3 t "trail arc #_1", \
      for [i=1:5] data i 3+i u 1:2:3 w l lc 3 lw 3 \
      t sprintf("new trail arc #_{1(%d)}",i), \
      data i 9 u 1:2:3 w lp lc 4 ps 3 pt 6 lw 0.5 t "trajectory_1", \
      data i 10 u 1:2:3 w l lc 1 dt 2 t "great circle #_2", \
      data i 11 u 1:2:3 w l lc 2 lw 3 t "trail arc #_2", \
      for [i=1:5] data i 11+i u 1:2:3 w l lc 3 lw 3 \
      t sprintf("new trail arc #_{2(%d)}",i), \
      data i 17 u 1:2:3 w lp lc 4 ps 3 pt 6 lw 0.5 t "trajectory_2"
