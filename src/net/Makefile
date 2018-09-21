#
# OMNeT++/OMNEST Makefile for libft_cup_omnet4
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-lib -O out -I/home/vfcosta/omnet/mf2o4-beta2/contrib/utils -I/home/vfcosta/omnet/mf2o4-beta2/contrib/nic/radios -I../trace -I/home/vfcosta/omnet/mf2o4-beta2/core/blackboard -I/home/vfcosta/omnet/mf2o4-beta2/core/basicMessages -I/home/vfcosta/omnet/mf2o4-beta2/contrib/applLayer -I/home/vfcosta/omnet/mf2o4-beta2/contrib/netwLayer -I/home/vfcosta/omnet/mf2o4-beta2/core/basicModules -I/home/vfcosta/omnet/mf2o4-beta2/core/channelControl -I.. -I/home/vfcosta/omnet/mf2o4-beta2/core/utils -I/home/vfcosta/omnet/mf2o4-beta2/contrib/messages -I../util -I/home/vfcosta/omnet/mf2o4-beta2/contrib/nic/macLayer -I../fault -L/home/vfcosta/omnet/mf2o4-beta2/out/$(CONFIGNAME)/contrib -L/home/vfcosta/omnet/mf2o4-beta2/out/$(CONFIGNAME)/core -L../../out/$(CONFIGNAME)/src/trace -L../../out/$(CONFIGNAME)/src/util -L../../out/$(CONFIGNAME)/src/graph -L../../out/$(CONFIGNAME)/src/app -L../../out/$(CONFIGNAME)/src/consensus -L../../out/$(CONFIGNAME)/src/fault -lmfcontrib -lmfcore -KMF2O4_PROJ=/home/vfcosta/omnet/mf2o4-beta2
#

# Name of target to be created (-o option)
TARGET = libft_cup_omnet4$(A_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(MF2O4_PROJ)/contrib/utils \
    -I$(MF2O4_PROJ)/contrib/nic/radios \
    -I../trace \
    -I$(MF2O4_PROJ)/core/blackboard \
    -I$(MF2O4_PROJ)/core/basicMessages \
    -I$(MF2O4_PROJ)/contrib/applLayer \
    -I$(MF2O4_PROJ)/contrib/netwLayer \
    -I$(MF2O4_PROJ)/core/basicModules \
    -I$(MF2O4_PROJ)/core/channelControl \
    -I.. \
    -I$(MF2O4_PROJ)/core/utils \
    -I$(MF2O4_PROJ)/contrib/messages \
    -I../util \
    -I$(MF2O4_PROJ)/contrib/nic/macLayer \
    -I../fault \
    -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Output directory
PROJECT_OUTPUT_DIR = ../../out
PROJECTRELATIVE_PATH = src/net
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/FloodRouter.o $O/WuRouter.o $O/Net_m.o

# Message files
MSGFILES = \
    Net.msg

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
$O/FloodRouter.o: FloodRouter.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	FloodRouter.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/FloodRouter_n.o: FloodRouter_n.cc
$O/WuRouter_n.o: WuRouter_n.cc
$O/WuRouter.o: WuRouter.cc \
	../trace/TraceLog.h \
	WuRouter.h \
	$(MF2O4_PROJ)/core/utils/ModuleAccess.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleNetwLayer.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	$(MF2O4_PROJ)/core/basicModules/BasicModule.h \
	$(MF2O4_PROJ)/core/utils/SimpleAddress.h \
	$(MF2O4_PROJ)/contrib/netwLayer/SimpleArp.h \
	$(MF2O4_PROJ)/core/blackboard/Blackboard.h \
	Net_m.h \
	$(MF2O4_PROJ)/core/basicModules/BasicLayer.h \
	$(MF2O4_PROJ)/core/utils/MacControlInfo.h \
	../Configuration.h
$O/Net_m.o: Net_m.cc \
	../trace/TraceLog.h \
	$(MF2O4_PROJ)/contrib/messages/NetwPkt_m.h \
	../util/NeighborDetector.h \
	Net_m.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	../trace/Trace.h \
	../Configuration.h

