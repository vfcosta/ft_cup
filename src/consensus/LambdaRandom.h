#ifndef LAMBDARANDOM_H
#define LAMBDARANDOM_H

#include "Lambda.h"
#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"

class NeighborDetector;
class Node;

class LambdaRandom : public Lambda
{
public:
	virtual void initialize(int);
	void send();
	void recv(NetwPkt *);

protected:
	void init();
	void receivePhase1(Propose *);
	int random();
	void endLambda();

    virtual void handleLowerMsg(cMessage*);

};

#endif
