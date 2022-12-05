reset
set xrange[-5:5]
set yrange[-10:10]
set grid
set ylabel "y"
set xlabel "x"
set terminal png
set output "graph_func/graph.png"
plot (((cos(x))+((sin(x))**(2)))+(x))