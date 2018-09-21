#ifndef LEADERUP_H
#define LEADERUP_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "Response_m.h"
#include "View_m.h"

#define ALIVE_TIMER (simTime()+TIMEOUT-INC)
#define TIMEOUT (2.0)
#define INC (1.0)

class NeighborDetector;
class Node;

class LeaderUP : public SimpleNetwLayer
{
public:
	virtual void initialize(int);

private:
	NeighborDetector *punish;
	NeighborDetector *mship;
	int leader;
	int leaderAnt;

   	cMessage *delayTimer;
   	bool begin;
   	bool end;
   	bool stop;

protected:
	void init();
	void send(bool);
	void receiveAlive(View *);
	void receiveTimeout(NetwPkt *);
	void sendResult();
	int getLeader();
    virtual void handleSelfMsg(cMessage*);
    virtual void handleUpperMsg(cMessage*);
    virtual void handleLowerMsg(cMessage*);

};

#endif
