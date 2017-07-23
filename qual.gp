set terminal pdfcairo
set output "qual.pdf"

# Colors from colorbewer
# http://colorbrewer2.org/#type=qualitative&scheme=Paired&n=7
set style line 1 lw 1.5 pt 1 ps 0.75 lc rgb '#fdbf6f'
set style line 2 lw 1.5 pt 2 ps 0.75 lc rgb '#a6cee3'
set style line 3 lw 1.5 pt 2 ps 0.75 lc rgb '#1f78b4'
set style line 4 lw 1.5 pt 4 ps 0.75 lc rgb '#b2df8a'
set style line 5 lw 1.5 pt 4 ps 0.75 lc rgb '#33a02c'
set style line 6 lw 1.5 pt 6 ps 0.75 lc rgb '#fb9a99'
set style line 7 lw 1.5 pt 6 ps 0.75 lc rgb '#e31a1c'

set title "Results quality vs conditioning"

set logscale xy
set key bottom right
set xlabel "Dot product condition number"
set ylabel "Relative error"
set format x "10^{%T}"
set format y "10^{%T}"

plot "qual.dat" using 1:2 w p ls 1 title "Standard", \
     ""         using 1:3 w p ls 2 title "C++, std", \
     ""         using 1:4 w p ls 3 title "C++, comp",\
     ""         using 1:5 w p ls 4 title "C, std",   \
     ""         using 1:6 w p ls 5 title "C, comp",  \
     ""         using 1:7 w p ls 6 title "F90, std", \
     ""         using 1:8 w p ls 7 title "F90, comp"
