#include "TraceLog.h"

TraceLog::TraceLog()
{
	sinkNodes.setName(SINK_NODES);
	sink.setName(SINK_CONVERGE);
	collect.setName(COLLECT_CONVERGE);
	decision.setName(DECISION_NODES);
	consensusTime.setName(CONSENSUS_TIME);
	sinkTime.setName(SINK_TIME);
	pd.setName(PD_CONVERGE);
	pdNodes.setName(PD_NODES);
	collectNodes.setName(COLLECT_NODES);
	agreement.setName(AGREEMENT);
	rounds.setName(ROUNDS);
}

void TraceLog::load() {
	//read information saved in file before runs
	FILE *f = fopen(statFile.c_str(),"r");
	if(f==NULL) {
		//create file if it not exists
		f = fopen(statFile.c_str(),"w");
	}
	else {
		collect.loadFromFile(f);
		sinkNodes.loadFromFile(f);
		sink.loadFromFile(f);
		decision.loadFromFile(f);
		consensusTime.loadFromFile(f);
		sinkTime.loadFromFile(f);
		pd.loadFromFile(f);
		pdNodes.loadFromFile(f);
		collectNodes.loadFromFile(f);
		agreement.loadFromFile(f);
		rounds.loadFromFile(f);
	}
	fclose(f);
}

void TraceLog::save() {
	//save statistics
	FILE *f = fopen(statFile.c_str(),"w");
	collect.saveToFile(f); // save the distribution
	sinkNodes.saveToFile(f);
	sink.saveToFile(f);
	decision.saveToFile(f);
	consensusTime.saveToFile(f);
	sinkTime.saveToFile(f);
	pd.saveToFile(f);
	pdNodes.saveToFile(f);
	collectNodes.saveToFile(f);
	agreement.saveToFile(f); // save the distribution
	rounds.saveToFile(f); // save the distribution
	fclose(f);
}

void TraceLog::setStatFile(std::string f) {
	statFile = f;
}


void TraceLog::collectSink(int v) {
	sink.collect(v);
}

void TraceLog::collectSinkNodes(int v) {
	sinkNodes.collect(v);
}

void TraceLog::collectCollect(int v) {
	collect.collect(v);
}

void TraceLog::collectCollectNodes(int v) {
	collectNodes.collect(v);
}

void TraceLog::collectPD(int v) {
	pd.collect(v);
}

void TraceLog::collectPDNodes(int v) {
	pdNodes.collect(v);
}

void TraceLog::collectDecision(int v) {
	decision.collect(v);
}

void TraceLog::collectConsensusTime(double v) {
	consensusTime.collect(v);
}

void TraceLog::collectSinkTime(double v) {
	sinkTime.collect(v);
}

void TraceLog::collectAgreement(int v) {
	agreement.collect(v);
}

void TraceLog::collectRounds(int v) {
	rounds.collect(v);
}

cStdDev TraceLog::getPDNodes() {
	return pdNodes;
}

cStdDev TraceLog::getPD() {
	return pd;
}

cStdDev TraceLog::getCollect() {
	return collect;
}

cStdDev TraceLog::getCollectNodes() {
	return collectNodes;
}

cStdDev TraceLog::getSink() {
	return sink;
}

cStdDev TraceLog::getSinkNodes() {
	return sinkNodes;
}

cStdDev TraceLog::getDecision() {
	return decision;
}

cStdDev TraceLog::getConsensusTime() {
	return consensusTime;
}

cStdDev TraceLog::getSinkTime() {
	return sinkTime;
}

cStdDev TraceLog::getAgreement() {
	return agreement;
}

cStdDev TraceLog::getRounds() {
	return rounds;
}
