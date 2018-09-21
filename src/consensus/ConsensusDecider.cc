#include "ConsensusDecider.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>

#include "../app/Application.h"
#include "SinkResult_m.h"
#include "View_m.h"

Define_Module(ConsensusDecider);

// do some initialization
void ConsensusDecider::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		decision = NIL;
		asked = new NeighborDetector();
		rec = new NeighborDetector();
		known = new NeighborDetector();
		iniTime = -1;
	}
	else if(stage==1){
	}
}

void ConsensusDecider::sink() {
	NetwPkt *pkt = new NetwPkt("INIT_SINK", Messages::INIT_SINK);
    pkt->setDestAddr(-1);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
	sendDown( pkt );
}


// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void ConsensusDecider::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
    case Messages::PROPOSE:
    	EV << "PROPOSE" << endl;
    	iniTime = simTime();
        sink();
		delete msg;
		break;
    default:
		sendDown(msg);
    }
}

//
void ConsensusDecider::init() {
	asked = new NeighborDetector();
	//decision = NIL;

	if(inSink) {
		EV << "fork agreement\n";
		agreement();
	}
	else {
		EV << "fork request\n";
		request();
	}
}

void ConsensusDecider::decide(int v) {
	if(decision!=NIL) return;

	decision = v;

//	std::map<int, Node>::iterator it;
//	std::map<int, Node> b = asked->getBuffer();
//	for(it = b.begin(); it != b.end(); it++)
//	{
//		Node n = it->second;
//		sendResponse(decision, n.getId());
//		EV << "sendResponse(" << decision << ", " << n.getId() << ")" << endl;
//	}
	sendResponse(decision, asked);
	//fim do consenso: envia decision para aplica��o
	sendDecision(decision);
}

void ConsensusDecider::sendDecision(int decision) {
//	if(FAULT->isFault(ID_CONS)) return;
	Response *v = new Response("CONSENSUS_RESULT", Messages::CONSENSUS_RESULT);
  	v->setDestAddr(ID_CONS);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setValue(decision);
	sendUp( v );
	sendDown( new Response(*v) );
	EV << "Decision: " << decision << "\n";
	TRACE_CONS->addDecision(decision);

	//nodes n�o iniciados n�o entram nesta estat�stica
	if(iniTime!=-1)
		TRACE_CONS->addConsensusTime((simTime()-iniTime).dbl());
}

void ConsensusDecider::receiveRequest(NetwPkt *n) {
	if(decision!=NIL) {
		sendResponse(decision, n->getSrcAddr());
		EV << "rsendResponse(" << decision << ", " << n->getSrcAddr() << ")" << endl;
	}
	else {
		asked->add(n->getSrcAddr());
		//EV << "asked: " << n->getSrcAddr() << "\n";
	}
}

void ConsensusDecider::request() {
//	std::map<int, Node>::iterator it;
//	std::map<int, Node> b = known->getBuffer();
//	for(it = b.begin(); it != b.end(); it++)
//	{
//		Node n = it->second;
//		sendRequest(n.getId());
//		//EV << "sendRequest("<<n.getId()<<")\n";
//	}
	sendRequest(known);
}

void ConsensusDecider::receiveResponse(Response *r) {
	if(decision==NIL) {
		decision = r->getValue();
		//fim do protocolo: publica decision para aplica��o
		EV << "response!" << endl;
		sendDecision(decision);
	}
}


void ConsensusDecider::handleLowerMsg( cMessage *msg )
{
 	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::CONSENSUS_RESULT: {
		//end of indulgent consensus!!
		Response *v = static_cast<Response *>(m);
		decide(v->getValue());
		delete msg;
	}
    break;
	case Messages::SINK_RESULT: {
		//FIM do sink!!
		SinkResult *v = static_cast<SinkResult *>(m);
		inSink = v->getInSink();
//		EV << "sink_result: " << inSink << endl;
		known = new NeighborDetector();
		known->copyFrom(v->getKnown());
		init();
		delete msg;
	}
    break;
	case Messages::REQUEST: {
		EV << "request" << endl;
		Response *v = static_cast<Response *>(m);
		if(v->getDest().contains(ID_CONS))
			receiveRequest(v);
		//EV << "Request received!\n";
		delete msg;
	}
    break;
	case Messages::RESPONSE: {
		EV << "response" << endl;
		Response *v = static_cast<Response *>(m);
//		if(v->getDest().contains(ID_CONS))
			receiveResponse(v);
		//EV << "Response received!\n";
		delete msg;
	}
    break;
//	case Messages::PHASE1: {
//		Propose *v = static_cast<Propose *>(m);
//		if(v->getDest().contains(ID_CONS)) {
////			EV << "Phase1 received!\n";
//			receivePhase1(v);
//		}
//		delete msg;
//	}
//    break;
//	case Messages::PHASE2: {
//		Propose *v = static_cast<Propose *>(m);
//		if(v->getDest().contains(ID_CONS)) {
////			EV << "Phase2 received!\n";
//			receivePhase2(v);
//		}
//		delete msg;
//	}
//    break;
//	case Messages::DECISION: {
//		Propose *v = static_cast<Propose *>(m);
//		//if(v->getDest().contains(ID_CONS)) {
//		if(inSink || v->getDest().contains(ID_CONS)) {
//			EV << "decision received!\n";
//			decide(v->getValue());
//		}
//		delete msg;
//	}
//    break;
//	case Messages::FD_RESULT: {
//		//FD envia novo conjunto de suspeitos
//		View *v = static_cast<View *>(m);
//		fd = new NeighborDetector();
//		fd->copyFrom(v->getKnown());
//
//		evaluatePhase1(eval1);
//		evaluatePhase2(eval2);
//
//		delete msg;
//	}
//    break;
    default:
		sendUp(msg);
    }
}


void ConsensusDecider::sendResponse(int value, int n) {
	NeighborDetector *dest = new NeighborDetector();
	dest->add(n);
	sendResponse(value, dest);
}

void ConsensusDecider::sendResponse(int value, NeighborDetector *dest) {
	if(dest->empty()) return;
	Response *v = new Response("RESPONSE", Messages::RESPONSE);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default

    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    v->setValue(value);
    v->setDest(*dest);
	sendDown( v );
}

void ConsensusDecider::sendRequest(NeighborDetector *dest) {
	if(dest->empty()) return;
	Response *v = new Response("REQUEST", Messages::REQUEST);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID_CONS );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setDest(*dest);
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
	sendDown( v );
}

void ConsensusDecider::agreement() {
	//send to lower layer a message to init real consensus in sink component
	View *pkt = new View("PROPOSE", Messages::PROPOSE);
    pkt->setDestAddr(-1);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);
    pkt->setKnown(*known);
	sendDown( pkt );
}
