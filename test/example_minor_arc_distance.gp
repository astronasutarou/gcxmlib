#!/usr/bin/env gnuplot
set terminal wxt size 800,800

set view equal xyz
set xr [-1:1]
set yr [-1:1]
set zr [-1:1]
data="< ./example_minor_arc_distance"

splot data i 0 u 1:2:3 w l lc 1 t sprintf("great circle #_%d",1), \
      data i 1 u 1:2:3 w l lw 3 lc 7 t sprintf("minor arc #_%d",1), \
      data i 2 u 1:2:3 w p pt 6 ps 2 lc 1 not, \
      data i 3 u 3:4:5 w p pt 6 lc 3 not, \
      data i 3 u 3:4:5:6:7:8 w vec lc 3 not, \
      data i 3 u 3:4:5:(sprintf("%.1f\n%.1f",$1,$2)) w labels font ',8' not

pause 3
