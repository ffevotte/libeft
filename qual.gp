set terminal pdfcairo
set output "qual.pdf"

set title "Results quality vs conditioning"

set logscale xy
set key bottom right
set xlabel "Dot product condition number"
set ylabel "Relative error"
set format x "10^{%T}"
set format y "10^{%T}"

plot "qual.dat" using 1:2 w p title "Standard", \
     ""         using 1:3 w p title "2sum", \
     ""         using 1:4 w p title "2prod",\
     ""         using 1:5 w p title "2sum + 2prod"
