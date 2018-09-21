#include "Sink.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <MacControlInfo.h>

#include "View_m.h"
#include "SinkResult_m.h"

Define_Module(Sink);

// do some initialization
void Sink::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage);

	if(stage==0){
		stop = false;
		end = false;
		begin = false;
		known = new NeighborDetector();
		requested = new std::set<SinkRequest *>();
	}
	else if(stage==1){
	}
}

void Sink::init() {
	responded = new NeighborDetector();

	std::set<SinkRequest *>::iterator itr;
	for(itr = requested->begin(); itr != requested->end(); itr++)
	{
		SinkRequest *m = *itr;
		receiveRequest(m);
	}


	//responded->add(ID);

//	std::map<int, Node>::iterator it;
//	std::map<int, Node> b = known->getBuffer();
//	for(it = b.begin(); it != b.end(); it++)
//	{
//		Node n = it->second;
//		sendRequest(known, n.getId());
//		//EV << "sendRequest to " << n.getId() << endl;
//	}
	sendRequest(known);
}

void Sink::receiveRequest(SinkRequest *m) {
	//espera acabar o collect para responder mensagens sink
	if(!begin) {
		requested->insert(m);
		return;
	}

//	NeighborDetector *ktemp = new NeighborDetector();
//	if(known!=NULL) ktemp->copyFrom(*known);
	//adicionando o pr�prio ID para compara��o
//	ktemp->add(ID);
//	m->getKnown().remove(ID);

//	NeighborDetector *mtemp = new NeighborDetector();
//	mtemp->copyFrom(m->getKnown());
//	mtemp->add(m->getSrcAddr());
	m->getKnown().add(m->getSrcAddr());
	known->add(ID);
	responded->add(m->getSrcAddr(), simTime().dbl());
	if(known!=NULL && m->getKnown().equals(*known)) {
//	if(m->getKnown().equals(*ktemp)) {
		EV << "sendAck to " << m->getSrcAddr() << endl;
		sendResponse(ACK, m->getSrcAddr());
	}
	else {
		EV << "sendNack to " << m->getSrcAddr() << endl;
		sendResponse(NACK, m->getSrcAddr());
	}
	requested->erase(m);
	known->remove(ID);
	delete m;
}


void Sink::receiveResponse(SinkResponse *m) {
	if(m->getAck()) {
		responded->add(m->getSrcAddr(), simTime().dbl());
		EV << "wait: " << ((known->size() - F)-responded->size()) << endl;
		if(responded->size() >= (known->size() - F)) {
			inSink = true;
			//fim do algoritmo: envia (inSink, known)
			sendSink(inSink, known);
		}
	}
	else {
		EV << "nack received from " << m->getSrcAddr() << endl;
		inSink = false;
		//fim do algoritmo: envia (inSink, known)
		sendSink(inSink, known);
	}
}

/**
 * fun��o responsavel por publicar os resultados
 * encontrados pelo algoritmo sink
 */
void Sink::sendSink(bool inSink, NeighborDetector *known) {
	if(!begin || end) return; //para n�o mandar mensagem mais de uma vez!

	SinkResult *v = new SinkResult("SINK_RESULT", Messages::SINK_RESULT);
  	v->setDestAddr(ID);
    v->setSrcAddr( ID );
//    v->setLength(headerLength);
//    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    v->setInSink(inSink);
    v->setKnown(*known);
	sendUp( v );
	EV << "END SINK: (" << inSink << ", "<< known->toString().data() << ")\n";
	end = true;
	if(inSink) TRACE->addSinkNodes(); //count sink nodes
	TRACE->addSink();
	TRACE->addSinkTime((simTime()-iniTime).dbl());
}


void Sink::collect() {
    NetwPkt *pkt = new NetwPkt("INIT_COLLECT", Messages::INIT_COLLECT);
    pkt->setDestAddr(L3BROADCAST);
    pkt->setSrcAddr( myNetwAddr );
//    pkt->setLength(headerLength);

    //pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    pkt->setControlInfo(new MacControlInfo(L2BROADCAST));
	sendDown( pkt );
}


void Sink::sendRequest(NeighborDetector *known) {
	if(known->empty()) return;
	SinkRequest *v = new SinkRequest("SINK_REQUEST", Messages::SINK_REQUEST);
    v->setDestAddr(L3BROADCAST);
    v->setSrcAddr( ID );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    v->setKnown(*known);
	sendDown( v );
}

void Sink::sendResponse(bool ack, int n) {
	SinkResponse *v = new SinkResponse("SINK_RESPONSE", Messages::SINK_RESPONSE);
    v->setDestAddr(n);
    v->setSrcAddr( ID );
//    v->setLength(headerLength);
    v->setTtl(-1); //ttl default
    v->setControlInfo( new NetwControlInfo(L3BROADCAST) );

    v->setAck(ack);
	sendDown( v );
}


void Sink::handleLowerMsg( cMessage *msg )
{
    NetwPkt *m = static_cast<NetwPkt *>(msg);
	if(stop || (m->getDestAddr()!=ID && m->getDestAddr()!=L3BROADCAST)) {
		EV << "delete handleLowerMsg" << endl;
		delete msg;
		return;
	}

    switch( msg->getKind() ){
    case Messages::SINK_REQUEST: {
		SinkRequest *r = static_cast<SinkRequest *>(m);
    	EV << "SINK_REQUEST " << r->getKnown().toString() << endl;
		if(r->getKnown().contains(ID)) {
			receiveRequest(r);
		}
        //delete msg;
    }
    break;
	case Messages::SINK_RESPONSE: {
		SinkResponse *res = static_cast<SinkResponse *>(m);
		receiveResponse(res);
        delete msg;
	}
    break;
	case Messages::COLLECT_RESULT: {
		//FIM do collect!!
		View *v = static_cast<View *>(m);
		known = new NeighborDetector();
		known->copyFrom(v->getKnown());
		EV << "Sink recebe dados do collect\n";
		begin = true;
		init();
        delete msg;
	}
    break;
    default:
		sendUp(msg);
    }
}


void Sink::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
	case Messages::CONSENSUS_RESULT: {
		//end of indulgent consensus!!
		stop = true;
		sendDown(msg);
	}
    break;
    case Messages::INIT_SINK: {
    	iniTime = simTime();
		collect(); //inicia o collect
		delete msg;
    }
	break;
    default:
		sendDown(msg);
    }
}

