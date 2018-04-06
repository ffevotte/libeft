set terminal pdfcairo size 5,5
set output "perf.pdf"

set title "Performance of the compensated dot product"

# Colors from ColorBrewer
#   http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=6
set style line 1 lw 3 lc rgb '#a6cee3'
set style line 2 lw 3 lc rgb '#1f78b4'
set style line 3 lw 3 lc rgb '#b2df8a'
set style line 4 lw 3 lc rgb '#33a02c'
set style line 5 lw 3 lc rgb '#fb9a99'
set style line 6 lw 3 lc rgb '#e31a1c'

set lmargin at screen 0.12
set multiplot

set key bottom left at 50,8e8 Left reverse

set size   1,0.6
set origin 0,0.4

set xrange [50:1e8]
set logscale x
set logscale y
set ylabel "FLOP/s"
set ytics format "10^{%T}"
set xtics format ""

plot "perfCxx.dat" u 1:3 w l ls 1 title "C++, native", \
     ""            u 1:2 w l ls 2 title "C++, comp", \
     "perfC.dat"   u 1:3 w l ls 3 title "C, native",\
     ""            u 1:2 w l ls 4 title "C, comp", \
     "perfF.dat"   u 1:3 w l ls 5 title "F90, native", \
     ""            u 1:2 w l ls 6 title "F90, comp"

set size   1,0.4
set origin 0,0

set key top right at 5e7,18

unset title
unset logscale y
set ylabel "Slow-down"
set yrange [*:*]
set ytics 0,5 format "%h"
set xlabel "Vector size (# elements)"
set xtics format "10^{%T}"

plot "perfCxx.dat" u 1:($3/$2) w l ls 2 title "C++", \
     "perfC.dat"   u 1:($3/$2) w l ls 4 title "C", \
     "perfF.dat"   u 1:($3/$2) w l ls 6 title "F"

unset multiplot