#ifndef LAMBDALEADER_H
#define LAMBDALEADER_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"
#include "Lambda.h"

class NeighborDetector;
class Node;

class LambdaLeader : public Lambda
{
public:
	virtual void initialize(int);
	void send();
	void recv(NetwPkt *);

private:
	NeighborDetector *fd;
	int prev_est1;
	int leader;
	int newLeader;
	std::set<Propose *> *requested;
	bool started;

protected:
	void init();
	void receivePhase1(Propose *);
	void leaderElection();
	void endLambda();

    virtual void handleLowerMsg(cMessage*);

};

#endif
