#ifndef PD_H
#define PD_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "View_m.h"

class NeighborDetector;
class Node;

class PD : public SimpleNetwLayer
{
public:
	virtual void initialize(int);
	void send();
	void sendResponse();
	void recv(View *, bool);

private:
	NeighborDetector *nd;
   	cMessage *delayTimer;
   	bool end;
   	bool begin;
   	bool timer;
   	bool twoHop;

protected:
	void sendPD(NeighborDetector *);

    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);
};

#endif
