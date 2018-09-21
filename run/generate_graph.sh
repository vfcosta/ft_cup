#! /bin/sh

source ./set_env.sh

if [ $# -lt 2 ]
then
echo "usage:"
echo "./generate_graph.sh INI_HOSTS MAX_HOSTS VAR_HOSTS F DIR"
exit 1
fi

INI_HOSTS=$1
MAX_HOSTS=$2
VAR_HOSTS=$3
F=$4
DIR=$5

#clean
#rm $DIR/decision-xgraph
#rm $DIR/consensustime-xgraph
#rm $DIR/sinknodes-xgraph
#rm $DIR/sinktime-xgraph

#rm $DIR/collectnodes-xgraph
#rm $DIR/collect-xgraph
#rm $DIR/pdnodes-xgraph
#rm $DIR/pd-xgraph
#rm $DIR/sink-xgraph

#generating xgraph
$GRAPH $INI_HOSTS $MAX_HOSTS $VAR_HOSTS $F $DIR
./plotscript.sh $DIR decision
./plotscript.sh $DIR consensustime
./plotscript.sh $DIR sinknodes
./plotscript.sh $DIR sinktime

./plotscript.sh $DIR collectnodes
./plotscript.sh $DIR collect
./plotscript.sh $DIR pdnodes
./plotscript.sh $DIR pd
./plotscript.sh $DIR sink

#touch files to ensure creation
touch $DIR/decision.eps
touch $DIR/consensustime.eps
touch $DIR/sinknodes.eps
touch $DIR/sinktime.eps
touch $DIR/collectnodes.eps
touch $DIR/collect.eps
touch $DIR/pdnodes.eps
touch $DIR/pd.eps
touch $DIR/sink.eps
