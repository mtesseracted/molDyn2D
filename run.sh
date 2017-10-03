#!/bin/bash
source inputs #import input variables
source progress.sh

#Compile executables
make main t=$time
exe1='time'$time'.exe'
make plots

#Optional command line arg:timestep size
./$exe1 $length $particles $vel0 $1 >results.dat 2>err.dat &
pid=$!
echo "Running molecular dynamics in PID "$pid

if [ -n $1 ]; then
    tstep=`grep '#define DELT*' main.cpp |head -1`
    tstep=($tstep)
    tstep=${tstep[2]}
else
    tstep=$1
fi
simSteps=`awk "BEGIN {print $time/$tstep}"`

stat=`kill -0 $pid 2>/dev/null;echo $?`
k=0 #Spinner dummy var
while [ "$stat" -eq 0 ]; do

    sleep $rfsh
    curr=`tail -1 results.dat`
    curr=($curr)
    curr=${curr[0]}
    curr=$((10#$curr)) #Strip leading zeros
    spi=${spin:k++%${#spin}:1}
    echo -ne "$(getBar $curr $simSteps 0 $SECONDS $spi)"
    stat=`kill -0 $pid 2>/dev/null; echo $?`
done
echo #newline
#gnuplot < plots.gnu #plot statistical data

echo -n "Molecular dynamics simulation complete. "
#echo -n "${pr1}$(formTime $SECONDS)"
echo

#Setup to render frames
mkdir movie
images=`wc -l < Pos.dat`
cp Pos.dat movie
./plots.exe $length $particles $images >movie.gnu
mv movie.gnu movie
cd movie

#Render in the background
tfs=$SECONDS #time frames start
gnuplot < movie.gnu &
pid=$!
echo
echo "Begining render of "$images" frames in PID "$pid
stat=`kill -0 $pid 2>/dev/null;echo $?`

k=0 #Spinner dummy var
while [ "$stat" -eq 0 ]; do

    sleep $rfsh
    curr=`ls -1tr |tail -1`
    curr=${curr:3:-4}
    curr=$((10#$curr)) #Strip leading zeros
    spi=${spin:k++%${#spin}:1}
    echo -ne "$(getBar $curr $images $tfs $SECONDS $spi)"
    stat=`kill -0 $pid 2>/dev/null; echo $?`
done
echo #newline

wait #Explicitly wait for safety
echo "Frame rendering complete."

#Make movie from frames
echo "Compiling Movie"
mencoder mf://*.png -ovc xvid -xvidencopts bitrate=2000 -fps $fps -o movie.avi
mv movie.avi ../
cd ../

#Clean Up
#rm movie/Pos.dat
#zip -r movieImages movie &>/dev/null #save frames
#rm -rf movie
#make clean
echo "::::::::::Program Complete::::::::::"
echo "Total ${pr1}$(formTime $SECONDS)"
#(EOF)
