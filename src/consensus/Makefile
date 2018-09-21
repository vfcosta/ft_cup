#
# OMNeT++/OMNEST Makefile for libft_cup_omnet4
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-lib -O out -I/home/vfcosta/omnet/mf2o4-beta2/contrib/utils -I/home/vfcosta/omnet/mf2o4-beta2/contrib/nic/radios -I/home/vfcosta/omnet/mf2o4-beta2/core/blackboard -I../trace -I/home/vfcosta/omnet/mf2o4-beta2/core/basicMessages -I/home/vfcosta/omnet/mf2o4-beta2/contrib/netwLayer -I/home/vfcosta/omnet/mf2o4-beta2/contrib/applLayer -I/home/vfcosta/omnet/mf2o4-beta2/core/basicModules -I/home/vfcosta/omnet/mf2o4-beta2/core/channelControl -I.. -I/home/vfcosta/omnet/mf2o4-beta2/core/utils -I/home/vfcosta/omnet/mf2o4-beta2/contrib/nic/macLayer -I/home/vfcosta/omnet/mf2o4-beta2/contrib/messages -I../util -I../fault -L/home/vfcosta/omnet/mf2o4-beta2/out/$(CONFIGNAME)/contrib -L/home/vfcosta/omnet/mf2o4-beta2/out/$(CONFIGNAME)/core -L../../out/$(CONFIGNAME)/src/net -L../../out/$(CONFIGNAME)/src/trace -L../../out/$(CONFIGNAME)/src/util -L../../out/$(CONFIGNAME)/src/graph -L../../out/$(CONFIGNAME)/src/app -L../../out/$(CONFIGNAME)/src/fault -lmfcontrib -lmfcore -KMF2O4_PROJ=/home/vfcosta/omnet/mf2o4-beta2
#

# Name of target to be created (-o option)
TARGET = libft_cup_omnet4$(A_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(MF2O4_PROJ)/contrib/utils \
    -I$(MF2O4_PROJ)/contrib/nic/radios \
    -I$(MF2O4_PROJ)/core/blackboard \
    -I../trace \
    -I$(MF2O4_PROJ)/core/basicMessages \
    -I$(MF2O4_PROJ)/contrib/netwLayer \
    -I$(MF2O4_PROJ)/contrib/applLayer \
    -I$(MF2O4_PROJ)/core/basicModules \
    -I$(MF2O4_PROJ)/core/channelControl \
    -I.. \
    -I$(MF2O4_PROJ)/core/utils \
    -I$(MF2O4_PROJ)/contrib/nic/macLayer \
    -I$(MF2O4_PROJ)/contrib/messages \
    -I../util \
    -I../fault \
    -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Output directory
PROJECT_OUTPUT_DIR = ../../out
PROJECTRELATIVE_PATH = src/consensus
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/LambdaFD.o \
    $O/CT_Cons.o \
    $O/LambdaRandom.o \
    $O/LeaderUP.o \
    $O/ConsensusDecider.o \
    $O/LambdaLeader.o \
    $O/GenericConsensus.o \
    $O/FDManet.o \
    $O/Lambda.o \
    $O/Sink.o \
    $O/PD.o \
    $O/Collect.o \
    $O/FD.o \
    $O/SinkResponse_m.o \
    $O/Response_m.o \
    $O/SinkRequest_m.o \
    $O/SinkResult_m.o \
    $O/View_m.o \
    $O/FDManetQR_m.o \
    $O/Propose_m.o

# Message files
MSGFILES = \
    SinkResponse.msg \
    Response.msg \
    SinkRequest.msg \
    SinkResult.msg \
    View.msg \
    FDManetQR.msg \
    Propose.msg

# Other makefile variables (-K)
MF2O4_PROJ=/home/vfcosta/omnet/mf2o4-beta2

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	ln -s -f $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@mkdir -p $O
	$(AR) $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f ft_cup_omnet4 ft_cup_omnet4.exe libft_cup_omnet4.so libft_cup_omnet4.a libft_cup_omnet4.dll libft_cup_omnet4.dylib
	-rm -f ./*_m.cc ./*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/FD_n.o: FD_n.cc
$O/LambdaFD_n.o: LambdaFD_n.cc
$O/PD_n.o: PD_n.cc
$O/Collect.o: Collect.cc \
	../trace/TraceLog.h \
	Collect.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	Sink.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../trace/Trace.h \
	../fault/FaultGenerator.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	SinkResponse_m.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	SinkRequest_m.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	PD.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/SinkRequest_m.o: SinkRequest_m.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	SinkRequest_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	../trace/Trace.h \
	../fault/FaultGenerator.h \
	../Configuration.h
$O/Response_m.o: Response_m.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Response_m.h \
	../Configuration.h
$O/View_m.o: View_m.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	../Configuration.h
$O/SinkResponse_m.o: SinkResponse_m.cc \
	../trace/TraceLog.h \
	SinkResponse_m.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	../trace/Trace.h \
	../fault/FaultGenerator.h \
	../Configuration.h
$O/LeaderUP_n.o: LeaderUP_n.cc
$O/Sink_n.o: Sink_n.cc
$O/FDManet.o: FDManet.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	FDManet.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	FDManetQR_m.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/LambdaLeader_n.o: LambdaLeader_n.cc
$O/LeaderUP.o: LeaderUP.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	LeaderUP.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	Response_m.h \
	../Configuration.h
$O/Consensus_n.o: Consensus_n.cc
$O/LambdaRandom_n.o: LambdaRandom_n.cc
$O/FD.o: FD.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	FD.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/LambdaRandom.o: LambdaRandom.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Propose_m.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	LambdaRandom.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	Response_m.h \
	../Configuration.h
$O/Propose_m.o: Propose_m.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Propose_m.h \
	../Configuration.h
$O/FDManetQR_m.o: FDManetQR_m.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	FDManetQR_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	../Configuration.h
$O/ConsensusDecider_n.o: ConsensusDecider_n.cc
$O/SinkResult_m.o: SinkResult_m.cc \
	../trace/TraceLog.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	../Configuration.h
$O/LambdaFD.o: LambdaFD.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Propose_m.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	Lambda.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	LambdaFD.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	Response_m.h \
	../Configuration.h
$O/Lambda.o: Lambda.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Propose_m.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	Lambda.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	Response_m.h \
	../Configuration.h
$O/ConsensusDecider.o: ConsensusDecider.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Propose_m.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	ConsensusDecider.h \
	Response_m.h \
	../Configuration.h
$O/CT_Cons_n.o: CT_Cons_n.cc
$O/FDManet_n.o: FDManet_n.cc
$O/LambdaLeader.o: LambdaLeader.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	LambdaLeader.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	Propose_m.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	Lambda.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	Response_m.h \
	../Configuration.h
$O/FT_CUP_n.o: FT_CUP_n.cc
$O/PD.o: PD.cc \
	../trace/TraceLog.h \
	Collect.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	PD.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/GenericConsensus.o: GenericConsensus.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	GenericConsensus.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	Propose_m.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	Response_m.h \
	../Configuration.h
$O/GenericConsensus_n.o: GenericConsensus_n.cc
$O/Cons_n.o: Cons_n.cc
$O/CT_Cons.o: CT_Cons.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../trace/Trace.h \
	../fault/FaultGenerator.h \
	Propose_m.h \
	CT_Cons.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	Response_m.h \
	../Configuration.h
$O/Sink.o: Sink.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	Sink.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	View_m.h \
	../trace/Trace.h \
	../fault/FaultGenerator.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	SinkResponse_m.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	SinkResult_m.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	SinkRequest_m.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/contrib/utils/NetwControlInfo.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/Collect_n.o: Collect_n.cc

