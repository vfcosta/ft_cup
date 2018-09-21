#include "FloodRouter.h"
#include <NetwPkt_m.h>
#include <MacControlInfo.h>
#include <Configuration.h>

Define_Module(FloodRouter);

#define IDF getParentModule()->getIndex()

/**
 * Reads all parameters from the ini file. If a parameter is not
 * specified in the ini file a default value will be set.
 **/
void FloodRouter::initialize(int stage)
{
    SimpleNetwLayer::initialize(stage);

    if(stage==0){
        //initialize seqence number to 0
        seqNum = 0;

        hasPar("defaultTtl") ? defaultTtl = par("defaultTtl") : defaultTtl = 5;
        hasPar("plainFlooding") ? plainFlooding = par("plainFlooding") : plainFlooding = true;

        EV << "defaultTtl = " << defaultTtl
           << " plainFlooding = " << plainFlooding << endl;

        if(plainFlooding){
            //these parameters are only needed for plain flooding
            hasPar("bcMaxEntries") ? bcMaxEntries = par("bcMaxEntries") : bcMaxEntries = 30;

            hasPar("bcDelTime") ? bcDelTime = par("bcDelTime") : bcDelTime = 3.0;
            EV <<"bcMaxEntries = "<<bcMaxEntries
               <<" bcDelTime = "<<bcDelTime<<endl;
        }
    }
}

void FloodRouter::finish() {
    if(plainFlooding){
		bcMsgs.clear();
    }
}


void FloodRouter::send(cMessage *msg) {
	scheduleAt(simTime()+delay(), msg);
//	sendDown(msg);
//	sendDelayed(msg, delay(), "lowergateOut");
}

float FloodRouter::delay() {
	//return 0.5+dblrand()/10;
	//return 0.1+dblrand()/100;
//	return 2*dblrand();

	return dblrand()*5; //it works!
}

void FloodRouter::handleSelfMsg(cMessage *msg)
{
	sendDown(msg);
//	delete msg;
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
void FloodRouter::handleUpperMsg(cMessage* m)
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
void FloodRouter::handleLowerMsg(cMessage* m)
{
    NetwPkt *msg = static_cast<NetwPkt *>(m);

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
            if( msg->getTtl() > 1 ){
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
            if( msg->getTtl() > 1 ){
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
bool FloodRouter::notBroadcasted( NetwPkt* msg )
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


void FloodRouter::sendUpFiltered( cMessage* msg ) {
    NetwPkt *m = dynamic_cast<NetwPkt *>(msg);
	if(m!=NULL && (m->getDestAddr()==IDF || m->getDestAddr()==L3BROADCAST)) {
		sendUp(msg);
		//EV << "sendUP: "<<m->getDestAddr();
	}
	else delete msg;
}
