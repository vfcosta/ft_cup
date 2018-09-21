#ifndef FD_H
#define FD_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>

#define ALIVE_TIMER (simTime()+TIMEOUT-INC)
#define TIMEOUT (0.2)
#define INC (0.1)

class NeighborDetector;
class Node;

class FD : public SimpleNetwLayer
{
public:
	virtual void initialize(int);

private:
	NeighborDetector *fd;
	NeighborDetector *known;
   	cMessage *delayTimer;
   	bool end;
   	bool begin;
	bool inSink;

protected:
	void init();
	void send();
	void receiveAlive(NetwPkt *);
	void receiveTimeout(NetwPkt *);

	void sendFD(NeighborDetector *);

    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

};

#endif
