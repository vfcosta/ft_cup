#!/bin/sh

SIMDIR=(../1hop_10_40_5_10_2_-1_0 ../1hop_10_40_5_10_2_-1_0.5 ../1hop_10_40_5_10_2_-1_1)
SIMNAME=("FT-CUP 0%" "FT-CUP 50%" "FT-CUP 100%")

. ../../combine_graphs.sh decision right bottom
. ../../combine_graphs.sh sinknodes right bottom
. ../../combine_graphs.sh sinktime left top
. ../../combine_graphs.sh consensustime left top

