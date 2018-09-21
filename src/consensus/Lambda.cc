#include "Lambda.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(Lambda);

// do some initialization
void Lambda::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!
	if(stage==0){
		begin = false;
		end = false;
	}
	else if(stage==1){
	}
}


void Lambda::init() {

}

void Lambda::endLambda() {
}

void Lambda::handleLowerMsg( cMessage *msg )
{
}

void Lambda::receivePhase1(Propose *p) {
}

void Lambda::sendResult() {
	EV << "[" << ID_CONS << "] sendResult: " << est2 << endl;
	begin = false;
	Response *r = new Response("LAMBDA_RESULT", Messages::LAMBDA_RESULT);
  	r->setDestAddr(ID_CONS);
    r->setSrcAddr( ID_CONS );
//    r->setLength(headerLength);
    r->setValue(est2);
	sendUp(r);
}


void Lambda::bcastPhase1(int r, int est) {
	bcastPhase1(r, est, -1);
}



void Lambda::bcastPhase1(int r, int est, int l) {
	EV << "[" << ID_CONS << "] bcastPhase1" << endl;
	Propose *v = new Propose("PHASE1", Messages::PHASE1);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
	v->setRound(r);
	v->setValue(est);
	v->setLeader(l);
	sendDown( v );
}

// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void Lambda::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
	case Messages::INIT_LAMBDA: {
		Propose *v = static_cast<Propose *>(msg);
		est1 = v->getValue();
		r = v->getRound();
		rec = new NeighborDetector();
		known->clear();
		known->copyFrom(v->getDest());
		begin = true;
		init();
		delete msg;
	}
	break;
	case Messages::END_LAMBDA: {
		EV << "[" << ID_CONS << "] END_LAMBDA" << endl;
		endLambda();
		sendDown(msg);
	}
    break;
    default:
		sendDown(msg);
    }
}
