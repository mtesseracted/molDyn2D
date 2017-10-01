#!/bin/bash
source inputs #import input variables

#Compile executables
make main t=$time
exe1='time'$time'.exe'
make plots

#Optional command line arg:timestep size
./$exe1 $length $particles $vel0 $1 >results.dat 2>err.dat
#gnuplot < plots.gnu #plot statistical data

#Make frames
mkdir movie
images=`wc -l < Pos.dat`
cp Pos.dat movie
./plots.exe $length $particles $images >movie.gnu
mv movie.gnu movie
cd movie
gnuplot < movie.gnu

#Make movie from frames
mencoder mf://*.png -ovc xvid -xvidencopts bitrate=2000 -fps $fps -o movie.avi
mv movie.avi ../
cd ../

#Clean Up
#rm movie/Pos.dat
#zip -r movieImages movie &>/dev/null #save frames
rm -rf movie
make clean



