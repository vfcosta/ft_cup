#ifndef TRACE_H_
#define TRACE_H_

#include <omnetpp.h>
#include "TraceLog.h"
#include <FaultGenerator.h>


/**
 * Generates statistics.
 */
class Trace : public cSimpleModule
{
  private:
  	TraceLog trace;

  	int sinkVar;
  	int sinkNodesVar;
  	int decisionVar;
  	int collectVar;
  	int pdVar;
  	int pdNodesVar;
  	int collectNodesVar;
  	int decisionValue;
  	int agreement;
  	int rounds;

  public:
    Trace();
   	void addSink();
   	void addSinkNodes();
   	void addDecision(int);
   	void addCollect();
   	void addCollectNodes(int);
   	void addPD();
   	void addPDNodes(int);
   	void addConsensusTime(double);
   	void addSinkTime(double);
   	void addRounds(int);
	void setStatFile(std::string);

  protected:
    virtual void initialize();
    virtual void finish();
};


#endif /*TRACE_H_*/
