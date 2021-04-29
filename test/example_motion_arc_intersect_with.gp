#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]

splot '< ./example_motion_arc_intersect_with' i 0 \
      u 1:2:3 w l lc 1 t sprintf("motion arc #_%d",1), \
      '< ./example_motion_arc_intersect_with' i 1 \
      u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      '< ./example_motion_arc_intersect_with' i 2 \
      u 2:3:($1==0?$4:1/0) w p pt 6 ps 0.2 lc 3 t 'not matched', \
      '< ./example_motion_arc_intersect_with' i 2 \
      u 2:3:($1==1?$4:1/0) w p pt 6 ps 0.5 lc 2 t 'matched'
pause -1
