cat << 'EOF' | gnuplot

set terminal pngcairo size 640,360 enhanced font 'DejaVuSansMono,8'

# set xrange [1:60000]
# set yrange [0:100]

set mytics 2
set grid mytics

set offsets 0.1, 0.1, 0.1, 0.1

# set xlabel ""
# set ylabel "accuracy"
# plot \
#     "aaaa" w l ls 8 t 'aaaa'
# plot \
#     "output" w p ls 8 t ''
# plot "< awk -F , '$3 == 1 ' train.csv" using 1:2 w p ls 8 ps 1 t '', \
#      "< awk -F , '$3 == -1' train.csv" using 1:2 w p ls 4 t '',  \
#      "< awk      '$3 == 1'  output"    using 1:2 w p ls 2 t '', \
#      "< awk      '$3 == -1' output"    using 1:2 w p ls 6 t ''
# set datafile separator ","

plot "< awk      '$3 == 1'  output"                       using 1:2 w p ls 10 pt 1 ps 0.5 lw 1 t '', \
     "< awk      '$3 == -1' output"                       using 1:2 w p ls 9  pt 1 ps 0.5 lw 1 t '', \
     "< awk -F , '$3 == 1'  train.csv | sed -e 's/,/ /g'" using 1:2 w p ls 6 ps 1.5 t '', \
     "< awk -F , '$3 == -1' train.csv | sed -e 's/,/ /g'" using 1:2 w p ls 8 ps 1.5 t ''

EOF
