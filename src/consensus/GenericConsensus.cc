#include "GenericConsensus.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(GenericConsensus);

// do some initialization
void GenericConsensus::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
//		initial = ID_CONS;
		initial = ID_CONS%2;
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

void GenericConsensus::init() {
	decision = NIL;
	begin = true;
	est1 = initial;
	r = 0;
	mainLoop();
}

void GenericConsensus::mainLoop() {
	EV << "[" << ID_CONS << "] r: " << r << endl;
	r++;
	//phase 1
	lambda(); //call lambda algorithm (blocks until terminates phase 1)
//	initPhase2(); //TODO: tirar daqui!!
}

//TODO: called when lambda terminates
void GenericConsensus::initPhase2() {
	EV << "[" << ID_CONS << "] initPhase2" << endl;
	bcastPhase2(r, est2);
}

void GenericConsensus::endPhase2() {
	rec = rrec.get(r);

	int v = rec->getValue();
	if(rec->values(NIL)) {
		EV << "[" << ID_CONS << "] NIL" << endl;
		est1 = NIL;
		mainLoop();
	}
	else
	if(rec->values(v)) {
		EV << "[" << ID_CONS << "] BCAST decision in sink component!" << endl;
		decide(v);
		bcastDecision(v);
	}
	else {
		EV << "[" << ID_CONS << "] consensus failed" << endl;
		EV << rec->toString() << endl;
		est1 = v;
		mainLoop();
	}
	eval2=true;
}

void GenericConsensus::lambda() {
	Propose *pkt = new Propose("INIT_LAMBDA", Messages::INIT_LAMBDA);
    pkt->setDestAddr(-1);
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
    pkt->setDest(*known);
    pkt->setValue(est1);
    pkt->setRound(r);
	sendDown( pkt );
}


void GenericConsensus::endLambda() {
	NetwPkt *pkt = new NetwPkt("END_LAMBDA", Messages::END_LAMBDA);
    pkt->setDestAddr(-1);
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
	sendDown( pkt );
}


void GenericConsensus::handleLowerMsg( cMessage *msg )
{
 	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(FAULT->isFault(ID_CONS) || (m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST)) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::LAMBDA_RESULT: {
		begin = true;
		if(decision!=NIL /*|| !begin*/) return;
		Response *v = static_cast<Response *>(m);
		est2 = v->getValue();
		EV << "LAMBDA_RESULT: " << est2 << endl;
		initPhase2();
		delete msg;
	}
	break;
	case Messages::PHASE2: {
		if(decision!=NIL /*|| !begin*/) return;
		Propose *v = static_cast<Propose *>(m);
		EV << "[" << ID_CONS << "] PHASE2: " << v->getDest().toString() << endl;
//		if(v->getDest().contains(ID_CONS)) {
			receivePhase2(v);
//		}
		delete msg;
	}
    break;
	case Messages::DECISION: {
		//!begin = fault node: bloquear mesmo?
		if(decision!=NIL || !begin) return;
		Propose *v = static_cast<Propose *>(m);
		if(begin || v->getDest().contains(ID_CONS)) {
			decide(v->getValue());
		}
		delete msg;
	}
    break;
    default:
		sendUp(msg);
    }
}

void GenericConsensus::receivePhase2(Propose *p) {
	rec = rrec.get(r);

	EV << "[" << ID_CONS << "] receivePhase2: " << p->getSrcAddr() << ", value: " << p->getValue() << endl;
	Node n;
	n.setId(p->getSrcAddr());
	n.setValue(p->getValue());
	rec->add(n);

	//evaluate phase 2
	std::map<int,Node> tmp = known->difference(rec->getBuffer());
	EV << "[" << ID_CONS << "] rec->size(): " << rec->size() << "; known->size(): " << known->size() << endl;
	//modifica��o para esperar n/2 mensagens
	if(rec->size() > known->size()/2) {
//	if(rec->size() > known->size() - F_CONS) {
//	if(tmp == fd->getBuffer() || tmp.empty()) {
		endPhase2(); //executes phase 2
	}
}

void GenericConsensus::bcastPhase2(int r, int est) {
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

void GenericConsensus::bcastDecision(int est) {
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

void GenericConsensus::decide(int v) {
	if(decision!=NIL) return;
	EV << "[" << ID_CONS << "] DECIDE" << endl;

	//trace rounds count (making 1 as min)
	if(r==0) {
		TRACE_CONS->addRounds(r+1);
	}
	else {
		TRACE_CONS->addRounds(r);
	}

	//tells lambda to finish
	endLambda();

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
void GenericConsensus::handleUpperMsg(cMessage* msg)
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
