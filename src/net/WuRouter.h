#ifndef WUROUTER_H_
#define WUROUTER_H_

#include <SimpleNetwLayer.h>
#include <list>

#include "Net_m.h"

class WuRouter : public SimpleNetwLayer
{
protected:
   	cMessage *delayTimer;
	NeighborDetector *nd;
	void receiveHello(Net *, bool);
	void sendHello();

    /** @brief Network layer sequence number*/
    unsigned long seqNum;

    /** @brief Default time-to-live (ttl) used for this module*/
    int defaultTtl;

    /** @brief Defines whether to use plain flooding or not*/
    bool plainFlooding;

    class Bcast {
    public:
        unsigned long seqNum;
        int srcAddr;
        simtime_t delTime;
    public:
        Bcast(unsigned long n=0, int s=0,  simtime_t d=0) :
            seqNum(n), srcAddr(s), delTime(d) {
        }
    };

    typedef std::list<Bcast> cBroadcastList;

    /** @brief List of already broadcasted messages*/
    cBroadcastList bcMsgs;

    /**
     * @brief Max number of entries in the list of already broadcasted
     * messages
     **/
    unsigned int bcMaxEntries;

    /**
     * @brief Time after which an entry for an already broadcasted msg
     * can be deleted
     **/
    double bcDelTime;

public:
    /** @brief Initialization of omnetpp.ini parameters*/
    virtual void initialize(int);
    virtual void finish();

protected:
	void send(cMessage *);

    float delay();

    /** @brief Handle messages from upper layer */
    virtual void handleUpperMsg(cMessage *);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage *);

    /** @brief we have no self messages */
    virtual void handleSelfMsg(cMessage* msg);

    /** @brief Checks whether a message was already broadcasted*/
    bool notBroadcasted( NetwPkt* );

	void sendUpFiltered( cMessage* );
};

#endif
