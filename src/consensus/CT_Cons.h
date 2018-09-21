#ifndef CT_CONS_H
#define CT_CONS_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "Propose_m.h"

class NeighborDetector;
class Node;

class CT_Cons : public SimpleNetwLayer
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

	bool eval1, eval2;

	bool begin;

protected:
	void failureDetector();
	void finishFD();
	void sendRequest(int);
	void sendResponse(int, int);
	void init();
	void sink();
	void request();
	void receiveResponse(Response *);
	void receiveRequest(NetwPkt *);
	void decide(int);
	void sendDecision(int);
	void propose();
	void bcastPhase1(int, int);
	void bcastPhase2(int, int);
	void bcastDecision(int);
	void receivePhase1(Propose *);
	void receivePhase2(Propose *);

	void evaluatePhase1(bool);
	void evaluatePhase2(bool);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

};

#endif
