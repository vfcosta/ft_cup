#include "LambdaRandom.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(LambdaRandom);

// do some initialization
void LambdaRandom::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		est2 = NIL;
		rec = new NeighborDetector();
		known = new NeighborDetector();
		est1 = NIL;
		r = 0;
		begin = false;
	}
	else if(stage==1){
	}
}

/**
 * random uses known sets to choice a randomized value
 */
int LambdaRandom::random() {
//	int i = dblrand()*rec->size();
//	EV << "i: " << i << endl;
//	int v = rec->index(i).getValue();
//	rec->clear();
//	return rec->size()>0?v:NIL;
	//returns 0 or 1
	return (int) (dblrand()*10)%2;
}

void LambdaRandom::init() {
	if(est1==NIL) {
		est1 = this->random();
		EV << "est1: " << est1 << endl;
	}
	bcastPhase1(r, est1);
}

void LambdaRandom::handleLowerMsg( cMessage *msg )
{
 	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(FAULT->isFault(ID_CONS) || (m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST)) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::PHASE1: {
		//ignore phase1 message when not begin (node not in sink) causes blocks in consensus protocol
//		if(!begin) return;
		Propose *v = static_cast<Propose *>(m);
//		if(v->getDest().contains(ID_CONS)) {
			receivePhase1(v);
//		}
		delete msg;
	}
    break;
    default:
		sendUp(msg);
    }
}

void LambdaRandom::receivePhase1(Propose *p) {
	EV << "receivePhase1: " << p->getSrcAddr() << "; v: " << p->getValue() << endl;
	rec = rrec.get(r);

	Node n;
	n.setId(p->getSrcAddr());
	n.setValue(p->getValue());
	rec->add(n);

	EV << "rec: " << rec->size() << "; kn: " << known->size() << "; F: " << F_CONS << endl;
	//	if(rec->size() > (known->size()-F_CONS)) {
	if(rec->size() > (known->size()/2)) {
		est2 = rec->getMajorityValue(); //est2 = v || est2 = NIL
		sendResult();
	}
}

void LambdaRandom::endLambda() {

}
