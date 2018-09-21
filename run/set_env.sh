#! /bin/sh

export PATH=$PATH:/home/vfcosta/omnet/omnetpp-4.0/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/vfcosta/omnet/omnetpp-4.0/lib:/home/vfcosta/omnet/mf2o4-beta2/core/lib:/home/vfcosta/omnet/mf2o4-beta2/contrib/lib
#FT_CUP=../bin/ft_cup
OMNETPP_INI=../src/omnetpp.ini

GRAPH=../src/graph/graph

FT_CUP="../src/ft_cup_omnet4 -u Cmdenv -n /home/vfcosta/omnet/omnet4_workspace/ft_cup_omnet4/simulations:/home/vfcosta/omnet/omnet4_workspace/ft_cup_omnet4/src:/home/vfcosta/omnet/mf2o4-beta2/contrib:/home/vfcosta/omnet/mf2o4-beta2/core:/home/vfcosta/omnet/mf2o4-beta2/networks:/home/vfcosta/omnet/mf2o4-beta2/scenarios:/home/vfcosta/omnet/mf2o4-beta2/testSuite -l /home/vfcosta/omnet/mf2o4-beta2/contrib/mfcontrib -l /home/vfcosta/omnet/mf2o4-beta2/core/mfcore"

SEEDTOOL=opp_lcg32_seedtool
