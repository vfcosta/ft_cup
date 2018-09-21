#! /bin/sh
source ./set_env.sh 

usage()
{
	echo "usage:"
	echo "./run.sh -i INI_HOSTS -m MAX_HOSTS -v VAR_HOSTS -r REPEAT -k DIV_K -f DIV_F -p DIV_FAULTS -d DIR -o OMNETPP_INI -a AREA [-l SEND_MAIL]"
	exit 1
}
#input parameters
DATE_INI="`date`"

while getopts 'ei:m:v:r:k:f:p:d:o:a:l' OPTION
do
  case $OPTION in
  i)	
	INI_HOSTS="$OPTARG"
	;;
  m)	
	MAX_HOSTS="$OPTARG"
	;;
  v)	
	VAR_HOSTS="$OPTARG"
	;;
  r)	
	REPEAT="$OPTARG"
	;;
  k)	
	DIV_K="$OPTARG"
	;;
  f)	
	DIV_F="$OPTARG"
	;;
  p)	
	DIV_FAULTS="$OPTARG"
	;;
  d)	
	DIR_SIM="$OPTARG"
	;;
  o)	
	OMNETPP_INI="$OPTARG"
	;;
  a)	
	AREA="$OPTARG"
	;;
  l)	
	SEND_MAIL=1
	;;
  e)	
	EXECUTE=1
	;;
	
  ?)	
  	usage;
	;;
  esac
done

if [ -f $MAX_HOSTS ]; then 
	usage;
fi


echo "Running FT-CUP with parameters:"
echo "INI_HOSTS: $INI_HOSTS"
echo "MAX_HOSTS: $MAX_HOSTS"
echo "VAR_HOSTS: $VAR_HOSTS"
echo "REPEAT: $REPEAT"
echo "DIV_K: $DIV_K"
echo "DIV_F: $DIV_F"
echo "DIV_FAULTS: $DIV_FAULTS"
echo "DIR_SIM: $DIR_SIM"
echo "DATE_INI: $DATE_INI"
echo "OMNETPP_INI: $OMNETPP_INI"
echo "AREA: $AREA"
echo "SEND_MAIL: $SEND_MAIL"

#fixed parameters
DIR=./output/$DIR_SIM
PARAMS=$DIR/params.ini

mkdir -p $DIR
cp $OMNETPP_INI $DIR/omnetpp.ini
cp ../src/omnetpp_general.ini $DIR

CMD="$0 $*"
echo $CMD > $DIR/command

#clean
rm $DIR/decision-xgraph
rm $DIR/consensustime-xgraph
rm $DIR/sinknodes-xgraph
rm $DIR/sinktime-xgraph
rm $DIR/*xgraph


#generating seeds
#seedtool g 1 10000000 $REPEAT > seeds.txt

if [ -f $EXECUTE ]
then
	REPEAT=1
fi



for ((numHosts=$INI_HOSTS; $numHosts<=$MAX_HOSTS; numHosts+=$VAR_HOSTS)); do

    echo "runing with $numHosts"
    K=`echo $numHosts / $DIV_K | bc`
    #F=`echo $K / $DIV_F | bc`
    F=`echo $numHosts \* $DIV_F | bc`
    echo "K: $K"
    echo "F: $F"

	#if k<0, use k = average neighbor
	if [ $DIV_K -lt 0 ]
	then
		num=`echo 3.1415*100*100 | bc`
	    dem=`echo $AREA/$numHosts | bc`
	    K=`echo $num/$dem | bc`
	    #K=`echo $K/2 | bc`
	    echo $num/$dem=$K
	fi

	if [ $DIV_F -lt 0 ]
	then
	    F=`echo $K + $DIV_F | bc`
	fi

	FAULTS=`echo $F \* $DIV_FAULTS | bc`
	#decimal bug!
	if [ $DIV_FAULTS == 0.5 ]
	then
	    FAULTS=`scale=0;echo $F / 2 | bc`
	fi
	echo "Faults: $FAULTS "

	(
	echo "[Config _$numHosts]"
	echo "sim.numHosts=$numHosts"
	echo "sim.trace.stat=\"$DIR/stat_$numHosts.dat"\"
	echo "sim.host[*].appl.K = $K"
	echo "sim.host[*].appl.F = $F"
	echo "sim.faultGenerator.F = $FAULTS"
	echo "output-scalar-file = $DIR/omnetpp.sca"
	echo "description = \"hosts: $numHosts rep: $REPEAT\""
	echo "repeat = $REPEAT"
	echo ""
	) >> $PARAMS

  for ((rep=0; $rep<$REPEAT; rep++)); do
	echo $DIR
	./generate_range_pattern.sh $numHosts $DIR $i 
	    
    if [ ! -f $EXECUTE ]
    then
    	echo $rep
    	$FT_CUP -c _$numHosts -r $rep -f $DIR/omnetpp.ini
    fi

    #./generate_graph.sh $INI_HOSTS $numHosts $VAR_HOSTS $FAULTS $DIR
  done 
  ./generate_graph.sh $numHosts $numHosts $numHosts $FAULTS $DIR
done

if [ ! -f $SEND_MAIL ] 
then
	/home/vfcosta/omnet/workspace/omnet_mail/send_mail $INI_HOSTS $MAX_HOSTS $VAR_HOSTS $REPEAT $DIV_K $DIV_F $DIV_FAULTS $DIR_SIM "$DATE_INI"
fi
