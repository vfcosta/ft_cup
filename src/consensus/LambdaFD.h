#ifndef LAMBDAFD_H
#define LAMBDAFD_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"
#include "Lambda.h"

class NeighborDetector;
class Node;

class LambdaFD : public Lambda
{
public:
	virtual void initialize(int);
	void send();
	void recv(NetwPkt *);

private:
	NeighborDetector *fd;
	int prev_est1;
	int cord;
	std::set<Propose *> *requested;

protected:
	void endLambda();
	void init();
	void receivePhase1(Propose *);
	void failureDetector();

    virtual void handleLowerMsg(cMessage*);

};

#endif
