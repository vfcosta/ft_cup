#include "Collect.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <MacControlInfo.h>

#include "Sink.h"
#include "PD.h"

Define_Module(Collect);

// do some initialization
void Collect::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		nd = new NeighborDetector();
		//PD
		PD = new NeighborDetector();
		known = new NeighborDetector();
		end = false;
		begin = false;
		stop = false;
		requested = new std::set<View *>();
	}
	else if(stage==1){
	}
}

//sends a hello message
void Collect::init() {
	//send messages stored in buffer
	std::set<View *>::iterator itr;
//	std::set<View *> *tmp = new std::set<View *>(*requested);
	for(itr = requested->begin(); itr != requested->end(); itr++)
	{
		View *m = *itr;
		recv(m);
		delete m;
	}
	requested->clear();

	known->copyFrom(*PD);
	responded = new NeighborDetector();
	previously_known = new NeighborDetector();
	//responded->add(ID);
	inquiry();
}

void Collect::inquiry() {
	std::map<int,Node> tmp = known->difference(previously_known->getBuffer());
	//to optimization, sends only one message to all destination (like a multicast)
	NeighborDetector *ntmp = new NeighborDetector();
	ntmp->setBuffer(tmp);
	send(ID, known, ntmp);

	wait = known->difference(responded->getBuffer()).size() - F;
	EV << "known: " << known->size() << "; previously_known: " << previously_known->size() << "; wait: " << wait << "\n";
	EV << "known: " << known->toString() << endl;
	EV << "previously_known: " << previously_known->toString() << endl;
	previously_known->clear();
	previously_known->copyFrom(*known);
}

void Collect::send(int initiator, NeighborDetector *known, int d) {
	NeighborDetector *dest = new NeighborDetector();
	dest->add(d);
	send(initiator, known, dest);
}

void Collect::send(int initiator, NeighborDetector *known, NeighborDetector *dest) {
	if(dest->empty()) return;
	View *v = new View("VIEW", Messages::VIEW);
    v->setDestAddr(L3BROADCAST);
//    v->setDestAddr(dest);
    v->setSrcAddr( ID );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );

    v->setInitiator(initiator);
    v->setKnown(*known);
    v->setDest(*dest);

	sendDown( v );
	delete dest;
}


/**
 * funcao responsavel por publicar o conjunto
 * encontrado pelo algoritmo COLLECT
 */
void Collect::sendCollect(NeighborDetector *known) {
	if(!begin || end) return; //para nao mandar mensagem mais de uma vez!

	View *v = new View("COLLECT_RESULT", Messages::COLLECT_RESULT);
  	v->setDestAddr(ID);
    v->setSrcAddr( ID );
//    v->setLength(headerLength);
    v->setKnown(*known);
	sendUp( v );
	EV << "END COLLECT: " << known->toString().data() << "\n";
	end = true;
	TRACE->addCollect();
	TRACE->addCollectNodes(known->size());
}

//receive a message from pj
void Collect::recv(View *m) {
	if(end) {
//		delete m;
//		return;
	}

	if(ID == m->getInitiator()) {
		if(responded->contains(m->getSrcAddr())) {
			delete m;
			return;
		}
		known->setBuffer(known->unions(m->getKnown().getBuffer()));
		known->remove(ID);
		previously_known->remove(ID);
		EV << "TRACE: " << known->size() << endl;
		EV << "TRACE PD: " << PD->size() << endl;
//		TRACE->setCollectNodes(known->size());

		responded->add(m->getSrcAddr(), simTime().dbl());
		wait--;
		//EV << "from " << m->getSrcAddr() << ": " << m->getKnown().toString() << "; known: "<< known->toString() << "; responded: "<< responded->toString() << "; wait: "<<wait<< endl;
		EV << "from " << m->getSrcAddr() << "; known: "<< known->toString() << "; responded: "<< responded->toString() << "; wait: "<<wait<< endl;
		if(wait<=0) {
			if(previously_known->equals(*known)) {
				//collect terminates here!!
				sendCollect(known);
			}
			else {
				inquiry();
			}
		}
		delete m;
	}
	else {
		if(!begin) {
			requested->insert(m);
		}
		else {
			EV << simTime() << " SENDM: " << m->getSrcAddr() << endl;
			send(m->getInitiator(), PD, m->getSrcAddr());
//			requested->erase(m);
//			delete m;
		}
	}
}

// You got a message from the MAC layer (most likely)
// take care of it, e.g. check whether you are the final
// receiver and so on
// You got a message from the network layer (most likely)
// take care of it
void Collect::handleLowerMsg( cMessage *msg )
{
    NetwPkt *m = static_cast<NetwPkt *>(msg);
	if(stop || (m->getDestAddr()!=ID && m->getDestAddr()!=L3BROADCAST)) {
		EV << "drop src: " << m->getSrcAddr() << "; dest: " << m->getDestAddr() << endl;
		delete msg;
		return;
	}

    switch( msg->getKind() ){
    case Messages::VIEW: {
    	View *v = static_cast<View *>(msg);
    	if(v->getDest().contains(ID)) {
    		recv(v);
    	}
    }
    break;
	case Messages::PD_RESULT: {
		//mensagem enviada pelo modulo pd que contem
		//o conjunto de nos detectados
		View *v = static_cast<View *>(msg);
		PD = new NeighborDetector();
		PD->copyFrom(v->getKnown());
		EV << "Collect recebe dados do PD: " << PD->toString() << endl;
		begin = true;
        init();
//        delete msg;
		sendUp(msg);
	}
    break;
    default:
		sendUp(msg);
    }
}


// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void Collect::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
	case Messages::CONSENSUS_RESULT: {
		//end of indulgent consensus!!
		stop = true;
		sendDown(msg);
	}
    break;
    case Messages::INIT_COLLECT:
        pd();
		delete msg;
		break;
    default:
		sendDown(msg);
    }
}


void Collect::pd() {
    NetwPkt *pkt = new NetwPkt("INIT_PD", Messages::INIT_PD);
    pkt->setDestAddr(L3BROADCAST);
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
    //pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    pkt->setControlInfo(new MacControlInfo(L2BROADCAST));
	sendDown( pkt );
}
