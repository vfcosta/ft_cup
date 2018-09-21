#ifndef SINK_H
#define SINK_H

#include <SimpleNetwLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>

#include "SinkResponse_m.h"
#include "SinkRequest_m.h"

class NeighborDetector;
class Node;

#define ACK true
#define NACK false

class Sink : public SimpleNetwLayer
{
public:
	virtual void initialize(int);

private:
	NeighborDetector *known;
	NeighborDetector *responded;
	bool inSink;

	bool end; //indicates end of sink algorithm
	bool begin; //indicates init of sink algorithm
	bool stop;

	std::set<SinkRequest *> *requested;

	simtime_t iniTime;
	double lastResponse;

protected:
	void send();
	void init();
	void receiveRequest(SinkRequest *);
	void receiveResponse(SinkResponse *);
	void sendRequest(NeighborDetector *);
	void sendResponse(bool, int);
	void collect();
	void sendSink(bool, NeighborDetector *);


    virtual void handleUpperMsg(cMessage*);

    virtual void handleLowerMsg(cMessage*);

};

#endif
