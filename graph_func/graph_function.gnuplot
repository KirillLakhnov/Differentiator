reset
set xrange[-10:10]
set yrange[-100:100]
set grid
set ylabel "y"
set xlabel "x"
set terminal png
set output "graph_func/graph.png"
plot (cos(x))