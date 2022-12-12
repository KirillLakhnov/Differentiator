reset
set xrange[-100:-10]
set yrange[-4:4]
set grid
set ylabel "y"
set xlabel "x"
set terminal png
set output "graph_func/graph.png"
plot ((x)*((log(x))**(2)))