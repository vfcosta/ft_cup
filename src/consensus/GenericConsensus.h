#ifndef GENERIC_CONSENSUS_H
#define GENERIC_CONSENSUS_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"
#include "RoundNodes.h"

class NeighborDetector;
class Node;

class GenericConsensus : public SimpleNetwLayer
{
public:
	virtual void initialize(int);
	void send();
	void recv(NetwPkt *);

private:
	int initial;
	NeighborDetector *known;
	int decision;
	NeighborDetector *fd;
	NeighborDetector *rec;
	int est1;
	int est2;
	int r;
	int cord;
	simtime_t iniTime;
	RoundNodes rrec;

	bool eval1, eval2;

	bool begin;

protected:
	void endLambda();
	void lambda();
	void mainLoop();
	void initPhase2();
	void endPhase2();

	void init();
	void decide(int);
	void sendDecision(int);
	void bcastPhase2(int, int);
	void bcastDecision(int);
	void receivePhase2(Propose *);


    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

};

#endif
