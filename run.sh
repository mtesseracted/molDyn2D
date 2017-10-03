#!/bin/bash
source inputs #import input variables

#Progress bar strings
pr1='Progress:['
pr2='% done'
prb='#######################################'
prs='                                       '
blen=${#prb}
spin="/-\|"
rfsh="0.18" #Refresh rate of progress bar

roundDiv () {
    #Integer division rounded to nearest integer
    d2=$(( $2 / 2 ))
    echo "$(( ($1+$d2) / $2))"
}

#Compile executables
make main t=$time
exe1='time'$time'.exe'
make plots

#Optional command line arg:timestep size
./$exe1 $length $particles $vel0 $1 >results.dat 2>err.dat
#gnuplot < plots.gnu #plot statistical data
echo "Molecular dynamics simulation complete."

#Make frames
mkdir movie
images=`wc -l < Pos.dat`
cp Pos.dat movie
echo "Begining render of "$images" frames."
./plots.exe $length $particles $images >movie.gnu
mv movie.gnu movie
cd movie

#Render frames in the background
gnuplot < movie.gnu &
pid=$!
echo "Rendering Frames in PID "$pid
stat=`kill -0 $pid 2>/dev/null;echo $?`
fpb=$( roundDiv $images ${#prb} ) #frames per bar

k=0 #Spinner dummy var
while [ "$stat" -eq 0 ]; do

    sleep $rfsh
    curr=`ls -1tr |tail -1`
    b10curr="10#""${curr:3: -4}" #Current frame number, base 10
    percent=$( roundDiv $((100*$b10curr)) $images )
    bcur=$( roundDiv $(($b10curr*$blen)) $images ) #bar current index
    str1=${pr1}"${prb:0:${bcur}}""${spin:k++%${#spin}:1}""${prs:$bcur}""]"${percent}${pr2}"\r"
    echo -ne "$str1"
    #echo -ne "Rendering Frame "$curr"\r"

    stat=`kill -0 $pid 2>/dev/null; echo $?`
#    sleep 1
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
rm -rf movie
make clean
#(EOF)
