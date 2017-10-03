set term pngcairo
unset key

#Position over all time plot
set output "particles.png"
p 'Pos.dat' u 1:2, 'Pos.dat' u 3:4, 'Pos.dat' u 5:6, 'Pos.dat' u 7:8, 'Pos.dat' u 9:10, 'Pos.dat' u 11:12, 'Pos.dat' u 13:14, 'Pos.dat' u 15:16, 'Pos.dat' u 17:18, 'Pos.dat' u 19:20, 'Pos.dat' u 21:22, 'Pos.dat' u 23:24, 'Pos.dat' u 25:26, 'Pos.dat' u 27:28, 'Pos.dat' u 29:30, 'Pos.dat' u 31:32
replot

#Velocity^2 plot
set output "Vel2.png"
p 'results.dat' u 1:7
replot

#Total Energy plot
set output "Energy.png"
p 'results.dat' u 1:9
replot

#Displacement^2 plot
set output "Disp2.png"
p 'results.dat' u 1:4
replot

#Displacements
set output "DispXY.png"
set key
p 'results.dat' u 1:2, 'results.dat' u 1:3
replot
unset key

#Velocity historgram over all time
set output "vel_hist2D.png"
binwidth=0.15
set boxwidth binwidth
bin(x,width)=width*floor(x/width) + binwidth/2.0
plot 'Vels.dat' using (bin($1,binwidth)):(1.0) smooth freq with boxes
replot

set output "vel_histX.png"
binwidth=0.15
set boxwidth binwidth
bin(x,width)=width*floor(x/width) + binwidth/2.0
plot 'Vels.dat' using (bin($2,binwidth)):(1.0) smooth freq with boxes
replot
set output "vel_histY.png"
binwidth=0.15
set boxwidth binwidth
bin(x,width)=width*floor(x/width) + binwidth/2.0
plot 'Vels.dat' using (bin($3,binwidth)):(1.0) smooth freq with boxes
replot

