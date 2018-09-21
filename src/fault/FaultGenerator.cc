#include "FaultGenerator.h"

Define_Module(FaultGenerator);

FaultGenerator::FaultGenerator() {
}

void FaultGenerator::initialize()
{
	faults = new NeighborDetector();
    if(getParentModule()->hasPar("numHosts")) {
    	nodes = getParentModule()->par("numHosts");
    }
    if(hasPar("F")) {
    	f = par("F");
    }
	generate();
}

void FaultGenerator::finish()
{
}


void FaultGenerator::generate() {
	while(faults->size()<f) {
		int n = (int) (dblrand() * 100)%nodes;
		faults->add(n);
	}
}

bool FaultGenerator::isFault(int n) {
	return faults->contains(n);
}
