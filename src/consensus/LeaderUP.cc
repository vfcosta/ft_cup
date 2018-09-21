#include "LeaderUP.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <MacControlInfo.h>

Define_Module(LeaderUP);

// do some initialization
void LeaderUP::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		begin = false;
		end = false;

		mship = new NeighborDetector();
		punish = new NeighborDetector();
		mship->add(ID_CONS);
		Node p;
		p.setId(ID_CONS);
		p.setValue(0);
		punish->add(p);
		leader = ID_CONS;
		leaderAnt = NIL;
	}
	else if(stage==1){
//		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
//		if(!FAULT->isFault(ID_CONS)) {
//			scheduleAt(ALIVE_TIMER, delayTimer);
//		}
	}
}


void LeaderUP::init() {
	send(true);
}

//sends a hello message
void LeaderUP::send(bool resend) {
	if(end || FAULT->isFault(ID_CONS)) return;

	View *alivePkt = new View("I_AM_ALIVE", Messages::I_AM_ALIVE);
    alivePkt->setDestAddr(L3BROADCAST);
    alivePkt->setSrcAddr( ID_CONS );
//    alivePkt->setLength(headerLength);
    alivePkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
    alivePkt->setTtl(-1);
	alivePkt->setKnown(*punish);
	sendDown(alivePkt);

	if(resend) {
		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
		scheduleAt(ALIVE_TIMER, delayTimer);
	}
}

void LeaderUP::receiveTimeout(NetwPkt *m) {
	int q = m->getSrcAddr();
	Node n = mship->get(q);
	n.setTime(n.getTime()+INC);
//	EV << "id: " << q << ";timeout: " << n.getTime() << endl;
	mship->add(n);
	punish->remove(q);
	send(false); //send but dont schedules
	leader = getLeader();
	sendResult();
}

void LeaderUP::receiveAlive(View *m) {
	if(end || !begin) return;
	if(FAULT->isFault(ID_CONS)) return;

//	EV << "[" << ID_CONS << "] receiveAlive: " << m->getSrcAddr() << endl;

	//TODO: flooding does that operation?
	//if (m.srcaddr, m.known) has not received before
//	send(); //resends q packet
	int q = m->getSrcAddr();
	Node n;
	n.setId(q);
	if(!mship->contains(q)) {
		n.setTime(TIMEOUT);
	}
	else {
		n.setTime(mship->get(q).getTime());
	}
	mship->add(n);
	//schedule
	NetwPkt *timeoutMessage = new NetwPkt( "FD_TIMEOUT", Messages::FD_TIMEOUT );
	timeoutMessage->setSrcAddr(n.getId());
	scheduleAt(simTime() + n.getTime(), timeoutMessage);

	Node sn = m->getKnown().get(q);
	if(punish->contains(q)) {
		Node pn = punish->get(q);
		Node n;
		n.setId(q);
		//VFC4: migration obs
//		n.setValue(max(sn.getValue(), pn.getValue()));
		n.setValue(sn.getValue()>pn.getValue()?sn.getValue():pn.getValue());
		punish->add(n);
	}
	else {
		Node n;
		n.setId(q);
		n.setValue(sn.getValue());
		punish->add(n);
	}

	if(!m->getKnown().contains(ID_CONS)) {
		Node p = punish->get(ID_CONS);
		p.setValue(p.getValue()+1);
		punish->add(p);
	}

	leader = getLeader();
	sendResult();
}


int LeaderUP::getLeader() {
	int leader = -1;
	int min=999;
	std::map<int, Node> b = punish->getBuffer();
	std::map<int, Node>::iterator it;
	for(it = b.begin(); it != b.end(); it++)
	{
		Node n = it->second;
		if(n.getValue()<=min) {
			min = n.getValue();
//			if(leader==-1 || leader >n.getId())
				leader = n.getId();
		}
	}
	return leader;
}

void LeaderUP::handleLowerMsg( cMessage *msg )
{
    View *m;
    switch( msg->getKind() ){
    case Messages::I_AM_ALIVE: {
    	if(end) return;
        m = static_cast<View *>(msg);
		receiveAlive(m);
        delete msg;
    }
    break;

    default:
		sendUp(msg);
    }
}


// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void LeaderUP::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
    case Messages::INIT_FD: {
        View *v = static_cast<View *>(msg);
		if(!begin) {
			EV << "[" << ID_CONS << "] Init leader" << endl;
			init();
			begin = true;
		}
		delete msg;
    }
    break;
	case Messages::CONSENSUS_RESULT: {
		//end of indulgent consensus!!
		end = true;
		sendDown(msg);
	}
    break;

    case Messages::END_FD: {
		end = true;
		delete msg;
    }
	break;
    default:
		sendDown(msg);
    }
}

// You have send yourself a message -- probably a timer,
// take care of it
void LeaderUP::handleSelfMsg(cMessage *msg)
{
	switch( msg->getKind() ){
    case Messages::SEND_TIMER: {
    	if(end) return;
        send(true);
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
 * fun��o responsavel por publicar o l�der
 * encontrado pelo algoritmo
 */
void LeaderUP::sendResult() {
	if(!begin || end) return;
	if(leader==leaderAnt) return;

	leaderAnt = leader;
	Response *v = new Response("FD_RESULT", Messages::FD_RESULT);
  	v->setDestAddr(ID_CONS);
    v->setSrcAddr(myNetwAddr);
//    v->setLength(headerLength);
    v->setValue(leader);
	sendUp(v);
}
