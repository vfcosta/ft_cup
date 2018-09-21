#include "FD.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <MacControlInfo.h>

#include "View_m.h"
#include "SinkResult_m.h"

Define_Module(FD);

// do some initialization
void FD::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		begin = false;
		end = false;
		inSink = true; //considerar inicialmente que o n� pertence ao sink
		fd = new NeighborDetector();
	}
	else if(stage==1){
		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
		if(!FAULT->isFault(ID_CONS)) {
			scheduleAt(ALIVE_TIMER, delayTimer);
		}
	}
}


void FD::init() {
	EV << "Init FD" << endl;
	fd = new NeighborDetector();
	std::map<int, Node> b = known->getBuffer();
	std::map<int, Node>::iterator it;
	for(it = b.begin(); it != b.end(); it++)
	{
		Node n = it->second;
		n.setTime(TIMEOUT);
		NetwPkt *timeoutMessage = new NetwPkt( "FD_TIMEOUT", Messages::FD_TIMEOUT );
		timeoutMessage->setSrcAddr(n.getId());
		scheduleAt(simTime() + n.getTime(), timeoutMessage);
	}
}

//sends a hello message
void FD::send() {
	if(end) return;

	//modificar o FD para enviar mensagens apenas se o n� for considerado Sink
//	if(!inSink) return;

    NetwPkt *pkt = new NetwPkt("I_AM_ALIVE", Messages::I_AM_ALIVE);
    pkt->setDestAddr(L3BROADCAST);
	pkt->setSrcAddr( ID_CONS );
//    pkt->setLength(headerLength);
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
	pkt->setTtl(-1);
	sendDown(pkt);

	delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
	scheduleAt(ALIVE_TIMER, delayTimer);
}

void FD::receiveTimeout(NetwPkt *m) {
	if(fd->contains(m->getSrcAddr())) return;
	fd->add(m->getSrcAddr());
	EV << "Suspect " << m->getSrcAddr() << endl;
	sendFD(fd);
}

void FD::receiveAlive(NetwPkt *m) {
	if(end || !begin) return;
	if(!fd->contains(m->getSrcAddr())) return;

	fd->remove(m->getSrcAddr());
	EV << "Correct " << m->getSrcAddr() << endl;
	sendFD(fd);

	Node n = known->get(m->getSrcAddr());
	n.setTime(n.getTime() + INC);
	NetwPkt *timeoutMessage = new NetwPkt( "FD_TIMEOUT", Messages::FD_TIMEOUT );
	timeoutMessage->setSrcAddr(n.getId());
	scheduleAt(simTime() + n.getTime(), timeoutMessage);
}

void FD::handleLowerMsg( cMessage *msg )
{
    NetwPkt *m;
    switch( msg->getKind() ){
    case Messages::I_AM_ALIVE: {
        m = static_cast<NetwPkt *>(msg);
		receiveAlive(m);
        delete msg;
    }
    break;

    case Messages::SINK_RESULT: {
		//FIM do sink!!
		SinkResult *v = static_cast<SinkResult *>(msg);
		inSink = v->getInSink();

		if(!inSink) EV << "sink_result: " << inSink << "\n\n ";
		sendUp(msg);
	}
    break;

    default:
		sendUp(msg);
    }
}


// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void FD::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
    case Messages::INIT_FD: {
        View *v = static_cast<View *>(msg);
		begin = true;
		known = new NeighborDetector();
		known->copyFrom(v->getKnown());
		init();
		delete msg;
    }
    break;
    case Messages::END_FD: {
//		end = true;
		delete msg;
    }
	break;
    default:
		sendDown(msg);
    }
}

// You have send yourself a message -- probably a timer,
// take care of it
void FD::handleSelfMsg(cMessage *msg)
{
	switch( msg->getKind() ){
    case Messages::SEND_TIMER: {
        send();
		delete msg;
    }
	break;
    case Messages::FD_TIMEOUT: {
    	NetwPkt *m = static_cast<NetwPkt *>(msg);
		receiveTimeout(m);
		delete msg;
	}
	break;
    default:
		delete msg;
    }
}

/**
 * fun��o responsavel por publicar o conjunto
 * encontrado pelo algoritmo FD
 */
void FD::sendFD(NeighborDetector *k) {
	if(!begin || end) return;

	View *v = new View("FD_RESULT", Messages::FD_RESULT);
  	v->setDestAddr(ID_CONS);
    v->setSrcAddr(myNetwAddr);
//    v->setLength(headerLength);
    v->setKnown(*k);
	sendUp(v);
	EV << "SEND FD: " << fd->toString().data() << "\n";
}
