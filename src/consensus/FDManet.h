#ifndef FDMANET_H
#define FDMANET_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>
#include "FDManetQR_m.h"

using namespace std;

#define TIMEOUT (1.0)

class NeighborDetector;
class Node;

class FDManet : public SimpleNetwLayer
{
public:
	virtual void initialize(int);

private:
	int counter;
	int degree;

	NeighborDetector *suspected;
	NeighborDetector *mistake;
	NeighborDetector *KFD;
	NeighborDetector *R;
	NeighborDetector *oldSuspected;
	NeighborDetector *recFrom;

	cMessage *delayTimer;
   	bool end;
   	bool begin;
	bool inSink;

protected:
	void init();
	void sendQuery();
	void sendResponse(int);
	void receiveQuery(FDManetQR *);
	void receiveResponse(FDManetQR *);
	int mistakeDetector(int); //returns node who has detected the mistaked node in parameter

	void sendFD(NeighborDetector *); //sends result to upper layer

    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

};

#endif
