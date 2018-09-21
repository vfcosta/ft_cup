#ifndef TRACE_LOG_H_
#define TRACE_LOG_H_

#include <omnetpp.h>

#define SINK_NODES "Sink nodes"
#define SINK_CONVERGE "Sink converge"
#define SINK_TIME "Sink time"
#define COLLECT_CONVERGE "Collect converge"
#define DECISION_NODES "Decision nodes"
#define CONSENSUS_TIME "Consensus Time"
#define PD_CONVERGE "PD converge"
#define PD_NODES "PD nodes"
#define COLLECT_NODES "Collect nodes"
#define AGREEMENT "Agreement"
#define ROUNDS "Rounds"

class TraceLog
{
  private:
  	cStdDev collect;
  	cStdDev sinkNodes;
  	cStdDev decision;
  	cStdDev sink;
  	cStdDev consensusTime;
  	cStdDev sinkTime;
  	cStdDev pd;
  	cStdDev pdNodes;
  	cStdDev collectNodes;
  	cStdDev agreement;
  	cStdDev rounds;
  	std::string statFile;


  public:
    TraceLog();
  	void load();
  	void save();
	void setStatFile(std::string);

	void collectSink(int);
	void collectSinkNodes(int);
	void collectCollect(int);
	void collectDecision(int);
	void collectConsensusTime(double);
	void collectSinkTime(double);
	void collectPD(int);
	void collectPDNodes(int);
	void collectCollectNodes(int);
	void collectAgreement(int);
	void collectRounds(int);

	cStdDev getPDNodes();
	cStdDev getPD();
	cStdDev getCollectNodes();
	cStdDev getCollect();
	cStdDev getSink();
	cStdDev getSinkNodes();
	cStdDev getDecision();
	cStdDev getConsensusTime();
	cStdDev getSinkTime();
	cStdDev getAgreement();
	cStdDev getRounds();
};


#endif /*TRACE_LOG_H_*/
