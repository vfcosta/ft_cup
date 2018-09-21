#include "Trace.h"

// register module class with OMNeT++
Define_Module(Trace);

Trace::Trace()
{
	collectVar = 0;
	sinkNodesVar = 0;
	sinkVar = 0;
	decisionVar = 0;
	pdVar = 0;
	pdNodesVar = 0;
	collectNodesVar = 0;
}

void Trace::initialize()
{
    if(hasPar("stat")) {
		std::string statFile;
//    	statFile.append(par("stat"));
		cPar p = par("stat");
		statFile.append(p.stringValue());
		trace.setStatFile(statFile);
    }
    else {
		std::string statFile;
    	statFile.append("stat.dat");
		trace.setStatFile(statFile);
    }
    trace.load();
    decisionValue=-1;
    agreement=1;
}

void Trace::finish()
{
    trace.collectSink(sinkVar);
	trace.collectCollect(collectVar);
	trace.collectDecision(decisionVar);
	trace.collectSinkNodes(sinkNodesVar);
	trace.collectPD(pdVar);

	if(agreement!=-1)
		trace.collectAgreement(agreement);
//	trace.collectPDNodes(pdNodesVar);
//	trace.collectCollectNodes(collectNodesVar);
    /*
	trace.getSinkNodes().recordScalar("sinkNodes");
	trace.getSink().recordScalar("sink");
	trace.getCollect().recordScalar("collect");
	trace.getDecision().recordScalar("decision");
	trace.getConsensusTime().recordScalar("consensusTime");
	*/
	trace.save();
}

void Trace::addSinkNodes() {
	sinkNodesVar++;
}

void Trace::addDecision(int value) {
	if(decisionValue==-1) decisionValue = value;
	else if(decisionValue!=value) agreement=0;
	decisionVar++;
}

void Trace::addCollect() {
	collectVar++;
}

void Trace::addCollectNodes(int n) {
	trace.collectCollectNodes(n);
}

void Trace::addSink() {
	sinkVar++;
}

void Trace::addPD() {
	pdVar++;
}

void Trace::addPDNodes(int n) {
	trace.collectPDNodes(n);
}

void Trace::addConsensusTime(double time) {
	trace.collectConsensusTime(time);
}

void Trace::addSinkTime(double time) {
	trace.collectSinkTime(time);
}

void Trace::addRounds(int r) {
	trace.collectRounds(r);
}
