#Progress bar strings
#pr1='Progress:['
pr1='Time Elapsed: '
pr2='% done'
prb='#######################################'
prs='                                       '
blen=${#prb}
spin="/-\|"
rfsh="0.18" #Refresh rate of progress bar

roundDiv () {
#Integer division rounded to nearest integer
#arg1: numerator, arg2: denominator
    d2=$(( $2 / 2 ))
    echo "$(( ($1+$d2) / $2))"
}

twoDig () {
#Add leading 0 to single digits numbers, base 10
#arg1: decimal number
    if [ $1 -lt 10 ]; then
	echo "0$1"
    else
	echo $1
    fi
}

formTime () {
#form a time string of format MmSSs from seconds
#arg1: seconds
    min=$(($1/60))
    sec=$(($1-($min*60)))
    sec=$(twoDig $sec)
    echo "${min}m${sec}s"
}


getBar () {
#Create progress bar
#arg1: current position, arg2: max position
#arg3: start time, arg4: current time
#arg5: spinner character
    curr=$1
    total=$2
    t1=$3
    t2=$4
    sp=$5
    percent=$( roundDiv $((100*$curr)) $total )
    bcur=$( roundDiv $(($curr*$blen)) $total ) #bar current index
    etim=$(formTime $(($t2-$t1)))
    str1="${pr1}${etim}:[${prb:0:${bcur}}${sp}"
    str1+="${prs:$bcur}]${percent}${pr2}\r"
    echo "$str1"
}

