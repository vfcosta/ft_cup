#ifndef CONSENSUS_DECIDER_H
#define CONSENSUS_DECIDER_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"

class NeighborDetector;
class Node;

class ConsensusDecider : public SimpleNetwLayer
{
public:
	virtual void initialize(int);
	void send();
	void recv(NetwPkt *);

private:
	bool inSink;
	NeighborDetector *known;
	int decision;
	NeighborDetector *asked;
	NeighborDetector *rec;
	simtime_t iniTime;

	bool eval1, eval2;

protected:
	void agreement();
	void sendRequest(NeighborDetector *);
	void sendResponse(int, NeighborDetector *);
	void sendResponse(int, int);
	void init();
	void sink();
	void request();
	void receiveResponse(Response *);
	void receiveRequest(NetwPkt *);
	void decide(int);
	void sendDecision(int);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

};

#endif
