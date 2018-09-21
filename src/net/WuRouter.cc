#include "WuRouter.h"
#include <NetwPkt_m.h>
#include <MacControlInfo.h>
#include <Configuration.h>

Define_Module(WuRouter);

#define IDF getParentModule()->getIndex()
#define RESCHEDULE (simTime() + 2 + dblrand())

/**
 * Reads all parameters from the ini file. If a parameter is not
 * specified in the ini file a default value will be set.
 **/
void WuRouter::initialize(int stage)
{
    SimpleNetwLayer::initialize(stage);

    if(stage==0){
        //initialize seqence number to 0
        seqNum = 0;

        hasPar("defaultTtl") ? defaultTtl = par("defaultTtl") : defaultTtl = 5;
        hasPar("plainFlooding") ? plainFlooding = par("plainFlooding") : plainFlooding = true;

        EV << "defaultTtl = " << defaultTtl
           << " plainFlooding = " << plainFlooding << endl;

		nd = new NeighborDetector();

		if(plainFlooding){
            //these parameters are only needed for plain flooding
            hasPar("bcMaxEntries") ? bcMaxEntries = par("bcMaxEntries") : bcMaxEntries = 30;

            hasPar("bcDelTime") ? bcDelTime = par("bcDelTime") : bcDelTime = 3.0;
            EV <<"bcMaxEntries = "<<bcMaxEntries
               <<" bcDelTime = "<<bcDelTime<<endl;
        }
    }
	else if(stage==1){
		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
		scheduleAt(dblrand(), delayTimer);
	}
}

void WuRouter::finish() {
    if(plainFlooding){
		bcMsgs.clear();
    }
}

void WuRouter::receiveHello(Net *m, bool response) {
	int id = m->getSrcAddr();
	Node n = Node(id, simTime().dbl());
	n.getNeighbors()->addAll(m->getKnown().getBuffer());
	n.getNeighbors()->remove(IDF);
	nd->add(n);
	delete m;
//	EV << "receiveHello: " << nd->toString() << endl;
}

//sends a hello message
void WuRouter::sendHello() {
//	EV << "sendHello" << endl;
    Net *pkt = new Net("NET_HELLO", Messages::NET_HELLO);
    pkt->setDestAddr(L3BROADCAST);
	pkt->setSrcAddr( IDF );
//    pkt->setLength(headerLength);
    pkt->setControlInfo(new MacControlInfo(L2BROADCAST));
	pkt->setTtl(0);
	pkt->setKnown(*(nd->copy())); //2-hop
	sendDown(pkt);

	delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
	scheduleAt(RESCHEDULE, delayTimer);
}

void WuRouter::send(cMessage *msg) {
	//TODO: implement wu here!!!
	EV << "send: " << msg->getKind() << endl;

	//read Wu paper about MANET routing protocol
	NeighborDetector gateways;
	std::map<int, Node> buffer = nd->getBuffer();
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		bool has = true;
		Node n = it->second;
		std::map<int, Node>::iterator it2;
		for(it2 = buffer.begin(); it2 != buffer.end(); it2++)
		{
			Node n2 = it2->second;
			if(n==n2) continue;
			bool has = n2.getNeighbors()->contains(n.getId());
			if(!has) {
				//mark
				gateways.add(n);
				break;
			}
		}
		if(!has) continue;
	}
	//TODO: rule 1
	//TODO: rule 2

	Net *n = new Net("NET", Messages::NET);
	n->encapsulate((cPacket *) msg->dup());
	n->setDest(gateways);
    n->setControlInfo(new MacControlInfo(L2BROADCAST));

    scheduleAt(simTime()+delay(), n);
//	scheduleAt(simTime()+delay(), msg);
//	sendDown(msg);
//	sendDelayed(msg, delay(), "lowergateOut");
}

float WuRouter::delay() {
	//return 0.5+dblrand()/10;
	//return 0.1+dblrand()/100;
//	return 2*dblrand();

	return dblrand()*5; //it works!
}

void WuRouter::handleSelfMsg(cMessage *msg)
{
	switch( msg->getKind() ){
    case Messages::SEND_TIMER: {
        sendHello();
		delete msg;
    }
	break;
    default:
    	sendDown(msg);
    }
}

/**
 * All messages have to get a sequence number and the ttl filed has to
 * be specified. Afterwards the messages can be handed to the mac
 * layer. The mac address is set to -1 (broadcast address) because the
 * message is flooded (i.e. has to be send to all neighbors)
 *
 * In the case of plain flooding the message sequence number and
 * source address has also be stored in the bcMsgs list, so that this
 * message will not be rebroadcasted, if a copy will be flooded back
 * from the neigbouring nodes.
 *
 * If the maximum number of entries is reached the first (oldest) entry
 * is deleted.
 **/
void WuRouter::handleUpperMsg(cMessage* m)
{

    NetwPkt *msg = encapsMsg(m);

    msg->setSeqNum( seqNum );
    seqNum++;

    NetwPkt *tmp = dynamic_cast<NetwPkt *>(m);
    if(tmp!=NULL) {
		msg->setTtl(tmp->getTtl());
    	if(tmp->getTtl()==-1) msg->setTtl( defaultTtl );
    	msg->setSrcAddr(tmp->getSrcAddr());
    	msg->setDestAddr(tmp->getDestAddr());
    }

    if(plainFlooding && msg->getTtl()>0){
        if( bcMsgs.size() >= bcMaxEntries ){
            cBroadcastList::iterator it;

            //search the broadcast list of outdated entries and delete them
            for( it=bcMsgs.begin(); it!=bcMsgs.end(); ++it ){
                if( it->delTime < simTime() ){
                    bcMsgs.erase(it);
                    it--;
                    break;
                }
            }
            //delete oldest entry if max size is reached
            if( bcMsgs.size() >= bcMaxEntries ){
                EV <<"bcMsgs is full, delete oldest entry"<<endl;
                bcMsgs.pop_front();
            }
        }
        bcMsgs.push_back(Bcast(msg->getSeqNum(), msg->getSrcAddr(), simTime() + bcDelTime));
    }
    //there is no routing so all messages are broadacst for the mac layer
//    float delay = dblrand()/100;
    send(msg);
}

/**
 * Messages from the mac layer will be forwarded to the application
 * only if the are broadcast or destined for this node.
 *
 * If the arrived message is a broadcast message it is also reflooded
 * only if the tll field is bigger than one. Before the message is
 * handed back to the mac layer the ttl field is reduced by one to
 * account for this hop.
 *
 * In the case of plain flooding the message will only be processed if
 * there is no corresponding entry in the bcMsgs list (@ref
 * notBroadcasted). Otherwise the message will be deleted.
 **/
void WuRouter::handleLowerMsg(cMessage* m)
{
    Net *net = static_cast<Net *>(m);

    if(net->getKind()==Messages::NET_HELLO) {
    	receiveHello(net, true);
    	return;
    }

    NetwPkt *msg = (NetwPkt *) net->decapsulate();

	if(msg->getTtl()<=0 && msg->getDestAddr() == L3BROADCAST) {
	    sendUpFiltered( decapsMsg(msg) );
		return;
	}

    //msg not broadcastes yet
    if( notBroadcasted( msg ) ){
        //msg is for me
        if( msg->getDestAddr() == IDF ) {
            EV <<" data msg for me! send to Upper"<<endl;
            sendUpFiltered( decapsMsg(msg) );
        }
        //broadcast message
        else if( msg->getDestAddr() == L3BROADCAST ){
            //check ttl and rebroadcast
            if( msg->getTtl() > 1
            		&& net->getDest().contains(IDF)){ //WU FILTER
				NetwPkt *dMsg;
                EV <<" data msg BROADCAST! ttl = "<<msg->getTtl()
                   <<" > 1 -> rebroadcast msg & send to upper\n";
                msg->setTtl( msg->getTtl()-1 );
                dMsg = static_cast<NetwPkt*>(msg->dup());
                dMsg->setControlInfo(new MacControlInfo(L2BROADCAST));
//                float delay = dblrand()/100;
                send(dMsg);
            }
            else
                EV <<" max hops reached (ttl = "<<msg->getTtl()<<") -> only send to upper\n";

	    	// message has to be forwarded to upper layer
	    	sendUpFiltered( decapsMsg(msg) );
        }
        //not for me -> rebroadcast
        else{
            //check ttl and rebroadcast
            if( msg->getTtl() > 1
            		&& net->getDest().contains(IDF)) { //WU FILTER
                EV <<" data msg not for me! ttl = "<<msg->getTtl()
                   <<" > 1 -> forward\n";
                msg->setTtl( msg->getTtl()-1 );
                //sendDown( msg );

   				NetwPkt *dMsg;
                msg->setTtl( msg->getTtl()-1 );
                dMsg = static_cast<NetwPkt*>(msg->dup());
                dMsg->setControlInfo(new MacControlInfo(L2BROADCAST));
//                float delay = dblrand()/100;
                send(dMsg);
                delete msg;

            }
            else{
                //max hops reached -> delete
                EV <<" max hops reached (ttl = "<<msg->getTtl()<<") -> delete msg\n";
                delete msg;
            }
        }
    }
    else{
        EV <<" data msg already BROADCASTed! delete msg\n";
        delete msg;
    }
    delete net;
}

/**
 * The bcMsgs list is searched for the arrived message. If the message
 * is in the list, it was already broadcasted and the function returns
 * false.
 *
 * Concurrently all outdated (older than bcDelTime) are deleted. If
 * the list is full and a new message has to be entered, the oldest
 * entry is deleted.
 **/
bool WuRouter::notBroadcasted( NetwPkt* msg )
{
    if(!plainFlooding)
        return true;

    cBroadcastList::iterator it;

    //search the broadcast list of outdated entries and delete them
    for( it=bcMsgs.begin(); it!=bcMsgs.end(); it++ ){
        if( it->delTime < simTime() ){
            bcMsgs.erase(it);
            it--;
        }
        //message was already broadcasted
        if( (it->srcAddr==msg->getSrcAddr()) && (it->seqNum==msg->getSeqNum()) ){
            // update entry
            it->delTime = simTime() + bcDelTime;
            return false;
        }
    }

    //delete oldest entry if max size is reached
    if( bcMsgs.size() >= bcMaxEntries ){
        EV <<"bcMsgs is full, delete oldest entry\n";
        bcMsgs.pop_front();
    }

    bcMsgs.push_back(Bcast(msg->getSeqNum(), msg->getSrcAddr(), simTime() + bcDelTime));
    return true;
}


void WuRouter::sendUpFiltered( cMessage* msg ) {
    NetwPkt *m = dynamic_cast<NetwPkt *>(msg);
	if(m!=NULL && (m->getDestAddr()==IDF || m->getDestAddr()==L3BROADCAST)) {
		sendUp(msg);
		//EV << "sendUP: "<<m->getDestAddr();
	}
	else delete msg;
}
