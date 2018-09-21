#include "CT_Cons.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(CT_Cons);

// do some initialization
void CT_Cons::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		initial = ID_CONS;
		decision = NIL;
		fd = new NeighborDetector();
		cord = est2 = NIL;
		rec = new NeighborDetector();
		known = new NeighborDetector();
		r = 0;
		est1 = initial;
		iniTime = -1;
		begin = false;
	}
	else if(stage==1){
	}
}

//
void CT_Cons::init() {
	EV << "INIT" << endl;
//	asked = new NeighborDetector();
	decision = NIL;
	//make use of classical consensus
	begin = true;
	failureDetector();
	propose();
}

void CT_Cons::sink() {
	NetwPkt *pkt = new NetwPkt("INIT_SINK", Messages::INIT_SINK);
    pkt->setDestAddr(-1);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
	sendDown( pkt );
}

void CT_Cons::decide(int v) {
	if(decision!=NIL) return;
	EV << "DECIDE" << endl;

	//tells fd to finish
	finishFD();

	decision = v;
	//end of indulgent consensus: send decision to upper layer
	Response *r = new Response("CONSENSUS_RESULT", Messages::CONSENSUS_RESULT);
  	r->setDestAddr(ID_CONS);
    r->setSrcAddr( ID_CONS );
//    r->setLength(headerLength);
    r->setValue(decision);
	sendUp(r);
}

// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void CT_Cons::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
    case Messages::PROPOSE: {
		View *v = static_cast<View *>(msg);
    	iniTime = simTime();
		known = new NeighborDetector();
		known->copyFrom(v->getKnown());
    	//decider tells indulgent consensus to init protocol
        init();
		delete msg;
    }
	break;
    default:
		sendDown(msg);
    }
}

void CT_Cons::handleLowerMsg( cMessage *msg )
{
 	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::PHASE1: {
		if(decision!=NIL) return;
//		if(!begin) return;
		Propose *v = static_cast<Propose *>(m);
		if(v->getDest().contains(ID_CONS)) {
			receivePhase1(v);
		}
		delete msg;
	}
    break;
	case Messages::PHASE2: {
		if(decision!=NIL) return;
//		if(!begin) return;
		Propose *v = static_cast<Propose *>(m);
		if(v->getDest().contains(ID_CONS)) {
			receivePhase2(v);
		}
		delete msg;
	}
    break;
	case Messages::DECISION: {
		if(decision!=NIL) return;
//		if(!begin) return;
		Propose *v = static_cast<Propose *>(m);
		if(begin || v->getDest().contains(ID_CONS)) {
			decide(v->getValue());
		}
		delete msg;
	}
    break;
	case Messages::FD_RESULT: {
		if(decision!=NIL) return;
//		if(!begin) return;
		//FD envia novo conjunto de suspeitos
		View *v = static_cast<View *>(m);
		fd = new NeighborDetector();
		fd->copyFrom(v->getKnown());
		EV << "FD_RESULT: " << fd->toString() << endl;

		evaluatePhase1(eval1);
		evaluatePhase2(eval2);

		delete msg;
	}
    break;
    default:
		sendUp(msg);
    }
}



/***
 * CHANDRA AND TOUEG CONSENSUS
 */
void CT_Cons::failureDetector() {
	View *pkt = new View("INIT_FD", Messages::INIT_FD);
    pkt->setDestAddr(-1);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
    pkt->setKnown(*known);
	sendDown( pkt );
}

void CT_Cons::finishFD() {
	View *pkt = new View("END_FD", Messages::END_FD);
    pkt->setDestAddr(-1);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
	sendDown( pkt );
}

void CT_Cons::propose() {
	r++;
	eval1 = false;
	eval2 = false;

	NeighborDetector *ntmp = new NeighborDetector();
	ntmp->copyFrom(*known);
	ntmp->add(ID_CONS);
	int i = (r-1)%(ntmp->size());
	cord = ntmp->index(i).getId();
//	EV << "cord: " << cord << endl;
	if(ID_CONS==cord) {
		est2 = est1;
//		EV << "bcastPhase1" << endl;
		bcastPhase1(r, est2);
	}
	evaluatePhase1(false);
}

void CT_Cons::receivePhase1(Propose *p) {
	EV << "receivePhase1: " << p->getSrcAddr() << endl;
//	EV << "cord: " << cord << endl;
	//if(cord==NIL) propose();
	if(cord==p->getSrcAddr()) {
		est2 = p->getValue();
		bcastPhase2(r, est2);
		eval1 = true;
	}
	else {
		evaluatePhase1(false);
	}
}

void CT_Cons::evaluatePhase1(bool eval) {
	if(eval) return;

//	EV << "evaluatePhase1" << endl;

	if(fd->contains(cord)) {
		est2 = NIL;
		bcastPhase2(r, est2);
		eval1 = true;
	}
}

void CT_Cons::receivePhase2(Propose *p) {
	EV << "receivePhase2: " << p->getSrcAddr() << ", value: " << p->getValue() << endl;
	Node n;
	n.setId(p->getSrcAddr());
	n.setValue(p->getValue());
	rec->add(n);
	evaluatePhase2(false);
}

void CT_Cons::evaluatePhase2(bool eval) {
	if(eval) return;

	std::map<int,Node> tmp = known->difference(rec->getBuffer());

//	EV << "rec->size(): " << rec->size() << "; known->size(): " << known->size() << "; fd->size(): " << fd->size() << endl;

	EV << "rec->size(): " << rec->size() << "; known->size(): " << known->size() << endl;
	//modifica��o para esperar n/2 mensagens
	if(rec->size() > known->size()/2) {
//	if(rec->size() > 8) {
//	if(tmp == fd->getBuffer() || tmp.empty()) {
		int v = rec->getValue();
		if(rec->values(NIL)) {
			EV << "NIL" << endl;
			propose();
		}
		else
		if(rec->values(v)) {
			EV << "BCAST decision in sink component!" << endl;
			decide(v);
			bcastDecision(v);
		}
		else {
			EV << "consensus failed" << endl;
			EV << rec->toString() << endl;
			est1 = rec->getValue();
			propose();
		}
		eval2=true;
	}
}


void CT_Cons::bcastPhase1(int r, int est) {
//	EV << "bcastPhase1" << endl;
	Propose *v = new Propose("PHASE1", Messages::PHASE1);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
	v->setRound(r);
	v->setValue(est);
	v->setDest(*known);
	sendDown( v );
}

void CT_Cons::bcastPhase2(int r, int est) {
//	EV << "bcastPhase2" << endl;
	Propose *v = new Propose("PHASE2", Messages::PHASE2);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
	v->setRound(r);
	v->setValue(est);
	v->setDest(*known);
	sendDown( v );
}

void CT_Cons::bcastDecision(int est) {
	Propose *v = new Propose("DECISION", Messages::DECISION);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
	v->setValue(est);
//	EV << "knwon: " << known->toString() << endl;
	v->setDest(*known);
	sendDown( v );
}

