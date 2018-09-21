#
# OMNeT++/OMNEST Makefile for libft_cup_omnet4
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-lib -O out -I.. -I../fault -L/home/vfcosta/omnet/mf2o4-beta2/out/$(CONFIGNAME)/contrib -L/home/vfcosta/omnet/mf2o4-beta2/out/$(CONFIGNAME)/core -L../../out/$(CONFIGNAME)/src/net -L../../out/$(CONFIGNAME)/src/util -L../../out/$(CONFIGNAME)/src/graph -L../../out/$(CONFIGNAME)/src/app -L../../out/$(CONFIGNAME)/src/consensus -L../../out/$(CONFIGNAME)/src/fault -lmfcontrib -lmfcore -KMF2O4_PROJ=/home/vfcosta/omnet/mf2o4-beta2
#

# Name of target to be created (-o option)
TARGET = libft_cup_omnet4$(A_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = -I.. -I../fault -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Output directory
PROJECT_OUTPUT_DIR = ../../out
PROJECTRELATIVE_PATH = src/trace
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/TraceLog.o $O/Trace.o

# Message files
MSGFILES =

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
$O/Trace_n.o: Trace_n.cc
$O/Trace.o: Trace.cc \
	TraceLog.h \
	../util/NeighborDetector.h \
	../util/Node.h \
	../fault/FaultGenerator.h \
	Trace.h \
	../Configuration.h
$O/TraceLog.o: TraceLog.cc \
	TraceLog.h

