#include "LambdaLeader.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(LambdaLeader);

// do some initialization
void LambdaLeader::initialize(int stage)
{
	Lambda::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		est2 = NIL;
		fd = new NeighborDetector();
		known = new NeighborDetector();
		rec = new NeighborDetector();
		est1 = NIL;
		r = 0;
		leader = NIL;
		newLeader = NIL;
		requested = new std::set<Propose *>();
		started = false;
	}
	else if(stage==1){
	}
}

void LambdaLeader::leaderElection() {
	View *pkt = new View("INIT_FD", Messages::INIT_FD);
	pkt->setDestAddr(-1);
	// we use the host modules index() as a appl address
	pkt->setSrcAddr( myNetwAddr );
//	pkt->setLength(headerLength);
	sendDown( pkt );
}

void LambdaLeader::init() {
	EV << "[" << ID_CONS << "] init lambda" << endl;
	started = true;
	leader = NIL;
	rec->clear();
	if(est1==NIL) {
		est1 = prev_est1;
	}
	else {
		prev_est1 = est1;
	}

	std::set<Propose *>::iterator itr;
	for(itr = requested->begin(); itr != requested->end(); itr++)
	{
		Propose *m = *itr;
		receivePhase1(m);
		delete m;
	}
	requested->clear();

	leaderElection();
}

void LambdaLeader::handleLowerMsg( cMessage *msg )
{
 	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::PHASE1: {
//		if(!begin) return;
		if(!started) {
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
//		if(!begin) return;
		//receives a new leader
		Response *v = static_cast<Response *>(m);
		//first time
		if(leader == NIL) {
			leader = v->getValue();
			EV << "[" << ID_CONS << "] leader: " << leader << "\n";
			newLeader = leader;
			bcastPhase1(r, est1, leader);
		}
		else {
			newLeader = v->getValue();
		}
		delete msg;
	}
    break;
    default:
		sendUp(msg);
    }
}

void LambdaLeader::receivePhase1(Propose *p) {
	EV << "[" << ID_CONS << "] receivePhase1: " << p->getSrcAddr() << "; v: " << p->getValue() << "; l: " << p->getLeader()<< "; r: " << p->getRound() << endl;


	rec = rrec.get(r);

	Node n;
	n.setId(p->getSrcAddr());
	n.setValue(p->getValue());
	n.setCounter(p->getLeader()); //n.counter represents the leader
	rec->add(n);

	if(rec->size() > (known->size()/2)) {
		EV << "[" << ID_CONS << "] rec: " << rec->toString() << endl;
		EV << "[" << ID_CONS << "] leader sends: " << rec->contains(leader) << "; leader: " << leader << "; newLeader: " << newLeader << endl;
		if(rec->contains(leader) || leader != newLeader) {
			//majority of phase one msgs with an leader value received
			//and a phase one message received from that leader
			int l = rec->getMajorityCounter();
			int est1Leader = rec->get(l).getValue();
			EV << "[" << ID_CONS << "] majority leader: " << l << "; est1l: " << est1Leader << endl;
			if(l!=NIL && rec->contains(l)) {
				est2 = est1Leader;
			}
			else {
				est2 = NIL;
			}
			if(l==ID_CONS) {
				est2 = est1;
			}
		}
		sendResult(); //problema: quando está executando a fase 2, pode enviar um outro resultado que altere est2
	}
}

void LambdaLeader::endLambda() {
	EV << "[" << ID_CONS << "] end Lambda" << endl;
	View *pkt = new View("END_FD", Messages::END_FD);
	pkt->setDestAddr(-1);
	// we use the host modules index() as a appl address
	pkt->setSrcAddr( myNetwAddr );
//	pkt->setLength(headerLength);
	sendDown( pkt );
	end = true;
}
