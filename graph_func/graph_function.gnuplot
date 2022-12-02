reset
set grid
set ylabel "y"
set xlabel "x"
set terminal png
set output "graph_func/graph.png"
plot (exp(x))