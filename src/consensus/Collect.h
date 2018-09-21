#ifndef COLLECT_H
#define COLLECT_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "View_m.h"

class NeighborDetector;
class Node;

class Collect : public SimpleNetwLayer
{
public:
	virtual void initialize(int);

private:
	NeighborDetector *nd;

	NeighborDetector *PD;
	NeighborDetector *responded;
	NeighborDetector *previously_known;
	NeighborDetector *known;

	std::set<View *> *requested;
	int wait;
	bool begin;
	bool end;
	bool stop;

protected:
	void pd();
	void init();
	void inquiry();
	void recv(View *);
	void send(int, NeighborDetector *, int);
	void send(int, NeighborDetector *, NeighborDetector *);
	void sendCollect(NeighborDetector *);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

};

#endif
