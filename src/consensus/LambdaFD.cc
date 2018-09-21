#include "LambdaFD.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(LambdaFD);

// do some initialization
void LambdaFD::initialize(int stage)
{
	Lambda::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		est2 = NIL;
		fd = new NeighborDetector();
		known = new NeighborDetector();
		est1 = NIL;
		r = 1;
		begin = false;
		cord = NIL;
		requested = new std::set<Propose *>();
	}
	else if(stage==1){
	}
}

void LambdaFD::failureDetector() {
	View *pkt = new View("INIT_FD", Messages::INIT_FD);
   pkt->setDestAddr(-1);
   // we use the host modules index() as a appl address
   pkt->setSrcAddr( myNetwAddr );
//   pkt->setLength(headerLength);
   pkt->setKnown(*known);
   sendDown( pkt );
}

void LambdaFD::init() {
	EV << "[" << ID_CONS << "] init" << endl;

	//init failure detection
//	failureDetector();

	if(est1==NIL) {
		est1 = prev_est1;
	}
	else {
		prev_est1 = est1;
	}

	//updates coordinator
	NeighborDetector *ntmp = new NeighborDetector();
	ntmp->copyFrom(*known);
	ntmp->add(ID_CONS);
	int ix = (r)%(ntmp->size());
	cord = ntmp->index(ix).getId();
	EV << "[" << ID_CONS << "] cord: " << cord << endl;
	//if the node is the round coordinator, then bcastPhase1
	if(ID_CONS==cord) {
		bcastPhase1(r, est1);
		est2 = est1;
		sendResult(); //send result itself
	}
	delete ntmp;


	std::set<Propose *>::iterator itr;
	for(itr = requested->begin(); itr != requested->end(); itr++)
	{
		Propose *m = *itr;
		receivePhase1(m);
		delete m;
	}
	requested->clear();
	if(ID_CONS!=cord) failureDetector();
}

void LambdaFD::handleLowerMsg( cMessage *msg )
{
 	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::PHASE1: {
		if(!begin) {
			Propose *v = static_cast<Propose *>(m);
			requested->insert(v);
			return;
		}
		Propose *v = static_cast<Propose *>(m);
//		if(v->getDest().contains(ID_CONS)) {
			receivePhase1(v);
//		}
		delete msg;
	}
    break;
	case Messages::FD_RESULT: {
		if(!begin || end) return;
		//FD envia novo conjunto de suspeitos
		View *v = static_cast<View *>(m);
		fd->clear();
		fd->copyFrom(v->getKnown());
		EV << "[" << ID_CONS << "] FD_RESULT: " << fd->toString() << "; cord: " << cord << endl;
		if(fd->contains(cord)) {
			est2 = NIL;
			sendResult();
		}
		delete msg;
	}
    break;
    default:
		sendUp(msg);
    }
}

void LambdaFD::endLambda() {
	NetwPkt *pkt = new NetwPkt("END_FD", Messages::END_FD);
    pkt->setDestAddr(-1);
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
	sendDown( pkt );
	end = true;
}

void LambdaFD::receivePhase1(Propose *p) {
	NeighborDetector *ntmp = new NeighborDetector();
	ntmp->copyFrom(*known);
	ntmp->add(ID_CONS);
//	r = p->getRound();
	int ix = (r)%(ntmp->size());
	cord = ntmp->index(ix).getId();

	EV << "[" << ID_CONS << "] receivePhase1: " << p->getSrcAddr() << "; v: " << p->getValue() << "; cord: " << cord << endl;

	//phase1 message received from cord
	if(p->getSrcAddr()==cord) {
		est2 = p->getValue();
		sendResult();
	}
}
