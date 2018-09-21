/*
 * Problema:
 * se o n� n�o executa o consenso cl�ssico, este n� pode nunca ser suspeitado pelos outros n�s,
 * fazendo com que o FT-CUP fique bloqueado pois os n�s no sink esperam por uma mensagem dele
 *
 * uma corre��o seria usar um detector que use o conjunto de n�s do sink como o pi
 * neste caso, o n� que n�o executar o consenso cl�ssico, n�o responderia as mensagens do detector.
 * Assim, este seria suspeitado pelos outros.
 *
 */
#include "FDManet.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <MacControlInfo.h>

#include "View_m.h"
#include "SinkResult_m.h"

Define_Module(FDManet);

// do some initialization
void FDManet::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		begin = false;
		end = false;
		inSink = true; //considerar inicialmente que o n� pertence ao sink

		//prevent segmentation fault
		counter = 0;
		suspected = new NeighborDetector();
		mistake = new NeighborDetector();
		KFD = new NeighborDetector();
		recFrom = new NeighborDetector();
		R = new NeighborDetector();
		degree = -1;
	}
	else if(stage==1){
//BUG: if fault node never sends a query message, it will never being suspected!!
//		if(FAULT->isFault(ID_CONS)) {
//			init();
//		}
	}
}

void FDManet::init() {
	EV << "[" << ID_CONS << "]Init FD" << endl;
	counter = 0;
	suspected = new NeighborDetector();
	mistake = new NeighborDetector();
	KFD = new NeighborDetector();
	recFrom = new NeighborDetector();
	R = new NeighborDetector();
	KFD->add(ID_CONS); //adiciona o pr�prio n� a k
	recFrom->add(ID_CONS); //adiciona o pr�prio n� a recFrom

	//send query to range
	sendQuery();
}

//sends a QUERY message
void FDManet::sendQuery() {
//	if(end || FAULT->isFault(ID_CONS)) return;
	if(end) return;

	EV << "[" << ID_CONS << "] sendQuery" << endl;

	//initiates responses count
	recFrom->clear();
	recFrom->add(ID_CONS); //adiciona o pr�prio n� a recFrom
	R->clear();

	FDManetQR *pkt = new FDManetQR("FD_QUERY", Messages::FD_QUERY);
    pkt->setSuspected(*suspected);
    pkt->setMistake(*mistake);
    pkt->setDestAddr(L3BROADCAST);
	pkt->setSrcAddr( ID_CONS );
//    pkt->setLength(headerLength);
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
	pkt->setTtl(0); //ttl=0 do not uses flooding (send to range)
	sendDown(pkt);
}

//sends a RESPONSE message
void FDManet::sendResponse(int dest) {
	if(end || FAULT->isFault(ID_CONS)) return;

	EV << "[" << ID_CONS << "] sendResponse" << endl;

	FDManetQR *pkt = new FDManetQR("FD_RESPONSE", Messages::FD_RESPONSE);
    pkt->setDestAddr(dest);
    pkt->setReceived(*recFrom);
	pkt->setSrcAddr( ID_CONS );
//    pkt->setLength(headerLength);
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
//	pkt->setTtl(-1); //ttl default
    pkt->setTtl(0);
	sendDown(pkt);
}

void FDManet::receiveQuery(FDManetQR *m) {
	EV << "[" << ID_CONS << "] receiveQuery: " << m->getSrcAddr() << endl;

	//fault node sends query here!! (to correct bug)
	//sends only one message!
	if(FAULT->isFault(ID_CONS) && !end) {
		suspected = new NeighborDetector();
		sendQuery();
		end = true;
	}
	if(end || !begin) return;

	//updates K
	KFD->add(m->getSrcAddr());

	//aux = suspected + mistake
	NeighborDetector *aux = new NeighborDetector();
	aux->setBuffer(suspected->unions(mistake->getBuffer()));

	//for all (px, counterx) in m->suspected
	map<int, Node>::iterator it;
	map<int, Node> jsuspected = m->getSuspected().getBuffer();
	for(it=jsuspected.begin(); it!=jsuspected.end(); it++)
	{
		Node px = it->second;

		if(!aux->contains(px.getId()) || (aux->contains(px.getId()) && aux->get(px.getId()).getCounter() < px.getCounter())) {
			if(px.getId()==ID_CONS) {
//				Node pi = m->getSuspected().get(ID_CONS);
				Node pi;
				pi.setId(ID_CONS);
//				counter = max(counter, px.getCounter() + 1);
//				pi.setCounter(counter);

				pi.setCounter(px.getCounter() + 1);
				mistake->add(pi);
			}
			else {
				suspected->add(px);
				mistake->remove(px.getId());
			}
		}
	}

	//for all (px, counterx) in m->mistake
	map<int, Node>::iterator itm;
	map<int, Node> jmistake = m->getMistake().getBuffer();
	for(itm=jmistake.begin(); itm!=jmistake.end(); itm++)
	{
		Node px = itm->second;
		if(!aux->contains(px.getId()) || (aux->contains(px.getId()) && aux->get(px.getId()).getCounter() < px.getCounter())) {
			px.setParent(ID_CONS); //set px mistake detector to pi
			mistake->add(px);
			suspected->remove(px.getId());
		}
//		//mistake detector
//		if(!KFD->contains(mistakeDetector(px.getId())) && KFD->contains(px.getId())) {
////			EV << "mistakeDetector: " << px.getId() << endl;
//			KFD->remove(px.getId());
//		}
	}
//	EV << "suspected: " << suspected->toString() << endl;
//	EV << "KFD: " << KFD->toString() << endl;
//	EV << "mistake: " << mistake->toString() << endl;

//	suspected->add(m->getSuspected());
	sendResponse(m->getSrcAddr());
	delete aux;
}

void FDManet::receiveResponse(FDManetQR *m) {
	if(end || !begin) return;

	EV << "[" << ID_CONS << "] receiveResponse: " << m->getSrcAddr() << endl;

	//updates recFrom
	recFrom->add(m->getSrcAddr());
	//updates R at receive response time
//	R->add(m->getReceived());

//	EV << "recFrom->size(): " << recFrom->size() << endl;
	//blocks protocols until receive enought responses
//	int d=2*F_CONS;
	int f=F_CONS;
	int d = degree;
	//wait until responses>=d-f
	int wfd = d-f;
	if(wfd<1) wfd=1;
	if(recFrom->size()<(wfd)) return;

//	EV << "received enought responses" << endl;
//	EV << "R: " << R->toString() << endl;
//	EV << "suspected: " << suspected->toString() << endl;
//	EV << "KFD: " << KFD->toString() << endl;

	//add new suspected nodes
	NeighborDetector *aux = new NeighborDetector();
	aux->setBuffer(KFD->difference(recFrom->getBuffer())); // K - recFrom
	aux->setBuffer(aux->difference(suspected->getBuffer())); // (K-recFrom) - suspected
	map<int, Node>::iterator it2;
	map<int, Node> buf2 = aux->getBuffer();
	for(it2=buf2.begin(); it2!=buf2.end(); it2++)
	{
		Node pj = it2->second;

		if(mistake->contains(pj.getId())) {
			Node pjm = mistake->get(pj.getId());
//			counter = max(counter, pjm.getCounter()+1);
			mistake->remove(pjm.getId());

			pjm.setCounter(pjm.getCounter()+1);
			suspected->add(pjm);
		}
		else {
			pj.setCounter(0); //updates pj counter
			suspected->add(pj);
		}
	}
	counter++;

	//send query to restart detection
//	sendQuery();
	if(!delayTimer) //cancelAndDelete(delayTimer);
	{
		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
		scheduleAt(simTime()+TIMEOUT, delayTimer);
	}

	//returns result to upper layer
//	if(oldSuspected==NULL || (suspected->difference(oldSuspected->getBuffer()).size()>0))
	sendFD(suspected);

	oldSuspected = suspected; //oldSuspected are used to avoid send identical messages to upper layer
	delete aux;
}


int FDManet::mistakeDetector(int pm) {
	std::map<int, Node>::iterator it;
	std::map<int, Node> buf = mistake->getBuffer();
	for(it=buf.begin(); it!=buf.end(); it++)
	{
		Node n = it->second;
		if(n.getId()==pm) {
//			EV << "DET: " << det << endl;
			return n.getParent();
		}
	}
	return -1;
}

void FDManet::handleLowerMsg( cMessage *msg )
{

 	NetwPkt *m = static_cast<NetwPkt *>(msg);
	if(m->getDestAddr()!=ID_CONS && m->getDestAddr()!=L3BROADCAST) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
    case Messages::FD_QUERY: {
    	FDManetQR *m = static_cast<FDManetQR *>(msg);
		receiveQuery(m);
        delete msg;
    }
    break;

    case Messages::FD_RESPONSE: {
    	FDManetQR *m = static_cast<FDManetQR *>(msg);
		receiveResponse(m);
        delete msg;
    }
    break;

    case Messages::SINK_RESULT: {
		//FIM do sink!!
		SinkResult *v = static_cast<SinkResult *>(msg);
		inSink = v->getInSink();

		if(!inSink) EV << "sink_result: " << inSink << "\n\n ";
		sendUp(msg);
	}
    break;
	case Messages::PD_RESULT: {
		View *v = static_cast<View *>(msg);
		degree = v->getKnown().size();
		EV << "[" << ID_CONS << "] Fdmanet recebe dados do PD: " << degree << endl;
        delete msg;
	}
    break;

    default:
		sendUp(msg);
    }
}


// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void FDManet::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
    case Messages::INIT_FD: {
        View *v = static_cast<View *>(msg);
    	begin = true;
		init();
		delete msg;
    }
	break;
    case Messages::END_FD: {
    	EV << "END_FD" << endl;
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
void FDManet::handleSelfMsg(cMessage *msg)
{
	switch( msg->getKind() ){
    case Messages::SEND_TIMER: {
        sendQuery();
        delayTimer = NULL;
		delete msg;
    }
	break;
    default:
		delete msg;
    }
}

/**
 * fun��o responsavel por publicar o conjunto
 * encontrado pelo algoritmo FD
 */
void FDManet::sendFD(NeighborDetector *k) {
	if(!begin || end) return;

	View *v = new View("FD_RESULT", Messages::FD_RESULT);
  	v->setDestAddr(ID_CONS);
    v->setSrcAddr(myNetwAddr);
//    v->setLength(headerLength);
    v->setKnown(*k);
	sendUp(v);
	EV << "SEND FD: " << k->toString().data() << "\n";
}
