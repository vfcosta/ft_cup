#!/bin/sh

#SIMDIR=(../run_10_40_5_10_2_-1_0 ../run_10_40_5_10_2_-1_0.5 ../run_10_40_5_10_2_-1_1)
#SIMNAME=("FT-CUP 0%" "FT-CUP 50%" "FT-CUP 100%")

PREFIX=$1 #gateway | receive | drop | delay
HALIG=$2
VALIG=$3

EPS_NAME=$PREFIX.eps
#X_AXIS="Number of Nodes"
X_AXIS="Quantidade de Nós"
GNUPLOT=gnuplot

#setando o XRANGE
#if [ $# -gt 3 ]; then
#	XRANGE=$4
#fi

if [ $# -gt 4 ]; then
	X_AXIS=$4 
	X_TIC=$5
else
	X_AXIS="Quantidade de Nós"
	X_TIC=10
fi

#setando o PREFIX
if [ $PREFIX = "decision" ]; then
	#Y_AXIS="FT-CUP Convergence (%)"
	Y_AXIS="Convergência do FT-CUP (%)"
	YRANGE=0:100
elif [ $PREFIX = "consensustime" ]; then
	#Y_AXIS="FT-CUP Latency (s)"
	Y_AXIS="Latência do FT-CUP (s)"
	YRANGE=0:40
elif [ $PREFIX = "sinknodes" ]; then
	#Y_AXIS="Sink Convergence (%)"
	Y_AXIS="Convergência para Poço (%)"
	YRANGE=0:30
elif [ $PREFIX = "sinktime" ]; then
	Y_AXIS="Sink Latency (s)"
	YRANGE=0:25
elif [ $PREFIX = "collectnodes" ]; then
	Y_AXIS="Participantes Coletados (%)"
	YRANGE=0:60
elif [ $PREFIX = "collect" ]; then
	Y_AXIS="Collect convergence"
	YRANGE=0:100
elif [ $PREFIX = "pdnodes" ]; then
	Y_AXIS="Participantes Detectados (%)"
	YRANGE=0:60
elif [ $PREFIX = "pd" ]; then
	Y_AXIS="PD convergence"
	YRANGE=0:100
elif [ $PREFIX = "sink" ]; then
	Y_AXIS="Sink convergence"
	YRANGE=0:100
elif [ $PREFIX = "agreement" ]; then
	#Y_AXIS="Agreement"
	Y_AXIS="Verificação do Acordo"
	YRANGE=0:100
elif [ $PREFIX = "rounds" ]; then
	Y_AXIS="Rodadas"
	YRANGE=0:10
else
	#usage
	echo "PREFIX = gateway | receive | drop | delay"
	#exit 1
fi

PLOT="plot "
#gera o comando para o plot
for ((i=0; $i<${#SIMDIR[@]}; i++)); do
PLOT="$PLOT \"${SIMDIR[$i]}/$PREFIX-xgraph\" using 1:2:3 title \"${SIMNAME[$i]}\" with yerrorlines ls `expr $i + 1` "
if [ `expr $i + 1` -lt ${#SIMDIR[@]} ]; then
	PLOT="$PLOT , "
fi

done

#set default size
if [ -z $SCALE_V ] 
then
	SCALE_V=0.55
fi

if [ -z $SCALE_H ] 
then
	SCALE_H=0.65
fi

#******************************************************************************
#script do GNUPLOT
$GNUPLOT << EOF
#configuracao EPS
set term postscript eps color enhanced solid
#set term png color
set output "./$EPS_NAME"

#local da legenda
#set key right bottom
#set key left top
set key $HALIG $VALIG

#set size 1.0,1.0
set size $SCALE_H,$SCALE_V

#nome dos eixos
set xlabel "$X_AXIS" #font "Helvetica,20"
set ylabel "$Y_AXIS" #font "Helvetica,20"

set encoding iso_8859_1

#ranges
#set xrange [$XRANGE]
set xtic $X_TIC
set yrange [$YRANGE] 


#estilo das linhas
#set style line 1 linetype 1 linecolor rgb "red"   linewidth 3 pointtype 5 pointsize 2.0
#set style line 1 linetype 1 linewidth 2.0 pointtype 5 pointsize 2.0
set style line 1 linetype 1 linewidth 1.0 pointtype 5 pointsize 1.0
set style line 2 linetype 7 linewidth 1.0 pointtype 7 pointsize 1.0
set style line 3 linetype 3 linewidth 1.0 pointtype 9 pointsize 1.0
set style line 4 linetype 4 linewidth 1.0 pointtype 5 pointsize 1.0
set style line 5 linetype 9 linewidth 1.0 pointtype 11 pointsize 1.0
set style line 6 linetype 2 linewidth 1.0 pointtype 13 pointsize 1.0

`echo $PLOT`

EOF
#FIM do script GNUPLOT
#******************************************************************************
