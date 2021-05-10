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
data="< ./example_trail_match"
set table $arc
plot data i 4 u 1:2:3:4:5:6 with table
unset table

splot data i 0 u 1:2:3 w l lc 1 t sprintf("great circle #_%d",1), \
      data i 1 u 1:2:3 w pm3d not, \
      data i 2 u 1:2:3 w pm3d not, \
      data i 3 u 1:2:3:(6+int($0)%2) w p pt var ps 2 lc 1  not, \
      $arc ev ::0::1 u ($1==0?-2:1/0):3:4 \
      w lp pt 7 ps 2 lc 3 t "not matched", \
      $arc ev ::0::1 u ($1==0?-2:1/0):3:4 \
      w lp pt 7 ps 2 lc 2 t "matched", \
      for [m=0:100] $arc ev ::(2*m)::(2*m+1) \
      u ($1==0?$2:1/0):3:4:(6+int($0)%2) w lp pt var ps 1 lc 3 not, \
      for [m=0:100] $arc ev ::(2*m)::(2*m+1) \
      u ($1==1?$2:1/0):3:4:(6+int($0)%2) w lp pt var ps 1 lc 2 not
