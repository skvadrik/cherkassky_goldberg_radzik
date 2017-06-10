set terminal pngcairo dashed font "Courier,mono"

set output 'plot_PROBLEM_logscale.png'
set logscale x 2
set logscale y 10
set xlabel "Number of nodes (logscale)"
set ylabel "Number of scans (logscale)"
plot "data/gor1a.PROBLEM" using 1:2 lc rgb '#000000' lt 1 with lines smooth unique title "GOR1", \
     "data/lau.PROBLEM"   using 1:2 lc rgb '#000000' lt 2 with lines smooth unique title "LAU", \
     "data/lau1.PROBLEM"  using 1:2 lc rgb '#000000' lt 3 with lines smooth unique title "LAU1"

set output 'plot_PROBLEM.png'
set xlabel "Number of nodes"
set ylabel "Number of scans"
unset logscale
set yrange [*<-1:*]
plot "data/gor1a.PROBLEM" using 1:2 lc rgb '#000000' lt 1 with lines smooth unique title "GOR1", \
     "data/lau.PROBLEM"   using 1:2 lc rgb '#000000' lt 2 with lines smooth unique title "LAU", \
     "data/lau1.PROBLEM"  using 1:2 lc rgb '#000000' lt 3 with lines smooth unique title "LAU1"
