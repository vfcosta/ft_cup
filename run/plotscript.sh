#!/bin/sh

usage() {
  echo "*******************************************************************************"
  echo "Requer gnuplot 4.0"
  echo "usage: plotscript.sh SIMDIR PREFIX [RANGE]"
  echo ""
  echo "SIMDIR: diretório das simulações"
  echo "PREFIX: métrica"
  echo "RANGE: range (default 0:100)"
  echo "*******************************************************************************"
}


if [ $# -lt 2 ]; then
  usage
  exit 1
fi


SIMDIR=$1
PREFIX=$2 #gateway | receive | drop | delay
EPS_NAME=$PREFIX.eps
X_AXIS="Number of Nodes in Network"
GNUPLOT=gnuplot

#setando o XRANGE
echo $3
if [ $# -gt 2 ]; then
	XRANGE=$3
fi

#setando o PREFIX
if [ $PREFIX = "decision" ]; then
	Y_AXIS="Decision nodes (%)"
	YRANGE=0:100
elif [ $PREFIX = "consensustime" ]; then
	Y_AXIS="Consensus time"
elif [ $PREFIX = "sinknodes" ]; then
	Y_AXIS="Sink nodes (%)"
	YRANGE=0:100
elif [ $PREFIX = "sinktime" ]; then
	Y_AXIS="Sink time"

elif [ $PREFIX = "collectnodes" ]; then
	Y_AXIS="Collect nodes"
	YRANGE=0:100
elif [ $PREFIX = "collect" ]; then
	Y_AXIS="Collect convergence"
	YRANGE=0:100
elif [ $PREFIX = "pdnodes" ]; then
	Y_AXIS="PD nodes"
	YRANGE=0:100
elif [ $PREFIX = "pd" ]; then
	Y_AXIS="PD convergence"
	YRANGE=0:100
elif [ $PREFIX = "sink" ]; then
	Y_AXIS="Sink convergence"
	YRANGE=0:100
else
	usage
	echo "PREFIX = gateway | receive | drop | delay"
	exit 1
fi




#******************************************************************************
#script do GNUPLOT
$GNUPLOT << EOF
#configuracao EPS
set term postscript eps color enhanced solid
#set term png color
set output "$SIMDIR/$EPS_NAME"

#local da legenda
set key left top 
#set size 1.0,1.0
set size 0.7,0.7

#nome dos eixos
set xlabel "$X_AXIS" #font "Helvetica,20"
set ylabel "$Y_AXIS" #font "Helvetica,20"


#ranges
set xrange [$XRANGE]
set yrange [$YRANGE] 


#estilo das linhas
#set style line 1 linetype 1 linecolor rgb "red"   linewidth 3 pointtype 5 pointsize 2.0
#set style line 1 linetype 1 linewidth 2.0 pointtype 5 pointsize 2.0
set style line 1 linetype 1 linewidth 1.2 pointtype 5 pointsize 1.3
set style line 2 linetype 7 linewidth 1.2 pointtype 7 pointsize 1.3
set style line 3 linetype 3 linewidth 1.2 pointtype 9 pointsize 1.3
set style line 4 linetype 4 linewidth 1.2 pointtype 11 pointsize 1.3
set style line 5 linetype 5 linewidth 1.2 pointtype 13 pointsize 1.3


plot "$SIMDIR/$PREFIX-xgraph" using 1:2:3 title "FT-CUP" with yerrorlines ls 1

#plot "$SIMDIR/protoepidemic/$PREFIX-xgraph" using 1:2:3 title "EraMobile" with yerrorlines ls 1,\
     "$SIMDIR/protodp/$PREFIX-xgraph" using 1:2:3 title "DP" with yerrorlines ls 2,\
     "$SIMDIR/protodc/$PREFIX-xgraph" using 1:2:3 title "DCB" with yerrorlines ls 3

EOF
#FIM do script GNUPLOT
#******************************************************************************

