#! /bin/sh

source ./set_env.sh

MAX_HOSTS=$1
DIR=$2
RANDOM=`$SEEDTOOL g $3 10000000 1` 
PARAMS=$DIR/range_pattern.ini
rm -f $PARAMS

# max transmission power [mW] 0.0631655 is 25m, 0.252662 is around 50m, 1.57914 is 125m, 6.33 is 250m
range=(0.0631655 0.252662 1.57914 6.33)

echo "[General]" >> $PARAMS

for ((i=0; $i<$MAX_HOSTS; i++)); do
    r=$(($RANDOM%4))
	(
	    echo "sim.host[$i].nic.snrEval.transmitterPower=${range[$r]}"
	) >> $PARAMS
done
