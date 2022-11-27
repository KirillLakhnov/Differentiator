reset
set grid
set title "График вашей функции" font "Helvetica Bold, 20"
set terminal png
set output "graph_func/graph.png"
plot (((x**2)/3)+(5*x))