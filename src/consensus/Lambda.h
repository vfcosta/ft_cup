#ifndef LAMBDA_H
#define LAMBDA_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"
#include "RoundNodes.h"

class NeighborDetector;
class Node;

class Lambda : public SimpleNetwLayer
{
public:
	virtual void initialize(int);

protected:
	NeighborDetector *known;
	NeighborDetector *rec;
	int est1;
	int est2;
	int r;
	double iniTime;
	bool begin;
	bool end;
	RoundNodes rrec;

	virtual void init();
	virtual void endLambda();
	virtual void receivePhase1(Propose *);
    virtual void handleLowerMsg(cMessage*);

	void bcastPhase1(int, int);
	void bcastPhase1(int, int, int);
	void sendResult();
    virtual void handleUpperMsg(cMessage*);

};

#endif
