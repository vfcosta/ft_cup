#include "PD.h"
#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <MacControlInfo.h>

#include "Collect.h"

#define RESCHEDULE (simTime() + dblrand() + 1)

Define_Module(PD);

// do some initialization
void PD::initialize(int stage)
{
	SimpleNetwLayer::initialize(stage); //DO NOT DELETE!!

	if(stage==0){
		begin = false;
		end = false;
		timer = false;
		nd = new NeighborDetector();

		if(hasPar("twoHop")) {
        	twoHop = par("twoHop");
        }
        else {
			twoHop = false;
        }
	}
	else if(stage==1){
	}
}

//sends a hello message
void PD::send() {
	if(end) return;

    View *pkt = new View("HELLO", Messages::HELLO);
    pkt->setDestAddr(L3BROADCAST);
	pkt->setSrcAddr( ID );
//    pkt->setLength(headerLength);
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
	pkt->setTtl(0); //n�o executa o flooding!
	if(twoHop) pkt->setKnown(*(nd->copy())); //2-hop
	sendDown(pkt);
}

void PD::sendResponse() {
    View *pkt = new View("HELLO_RESPONSE", Messages::HELLO_RESPONSE);
    pkt->setDestAddr(L3BROADCAST);
	pkt->setSrcAddr( ID );
//    pkt->setLength(headerLength);
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST));
	pkt->setTtl(0); //n�o executa o flooding!
	if(twoHop) pkt->setKnown(*(nd->copy())); //2-hop
	sendDown(pkt);
//    float delay = dblrand()/10; //melhorou bastante em rela�ao a /2. para muitos nos (40), ficou bem ruim!
//    sendDelayed(pkt, delay, "lowergateOut");
}

void PD::recv(View *m, bool response) {
	if( end || !begin ) {
		if(!response)
			sendResponse(); //responde o hello
	}

	int id = m->getSrcAddr();
	nd->add(id, simTime().dbl());
	//2-hop
	if(twoHop) {
		nd->setBuffer(nd->unions(m->getKnown().getBuffer()));
		nd->remove(ID);
	}

//	TRACE->setPDNodes(nd->size());
	//if(timer && nd->size()==KN)
	//if(nd->size()==KN)
	if(timer) //dont depends of K param
	{
		sendPD(nd);
	}
	EV << nd->toString() << endl;

}

void PD::handleLowerMsg( cMessage *msg )
{
    switch( msg->getKind() ){
    case Messages::HELLO: {
        View *m = static_cast<View *>(msg);
       	recv(m, false);
        delete msg;
    }
    break;
    case Messages::HELLO_RESPONSE: {
        View *m = static_cast<View *>(msg);
       	recv(m, true);
        delete msg;
    }
    break;
    default:
		sendUp(msg);
    }
}


// You got a message from an application (most likely)
// It is your job to figure out how to forward it
void PD::handleUpperMsg(cMessage* msg)
{
    switch( msg->getKind() ){
    case Messages::INIT_PD: {
		begin = true;

		if(end) sendPD(nd);
		else send();

		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
		scheduleAt(RESCHEDULE, delayTimer);

		cMessage *endTimer = new cMessage( "end-timer", Messages::PD_RESULT );
		scheduleAt(simTime() + 2 /*.5*/, endTimer); //TODO: remove pd time from result (consensus time)

		delete msg;
    }
	break;
    default:
		sendDown(msg);
    }
}

// You have send yourself a message -- probably a timer,
// take care of it
void PD::handleSelfMsg(cMessage *msg)
{
	switch( msg->getKind() ){
    case Messages::SEND_TIMER: {
        send();
        //comentado para otimizar a simula�ao
//		delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
//		scheduleAt(RESCHEDULE, delayTimer);
		delete msg;
    }
	break;
    case Messages::PD_RESULT: {
		sendPD(nd);
		timer = true;
		delete msg;
    }
	break;
    default:
		delete msg;
    }
}

/**
 * fun��o responsavel por publicar o conjunto
 * encontrado pelo algoritmo PD
 */
void PD::sendPD(NeighborDetector *k) {
	if(!begin || end /*|| k->size()<KN*/) return;

	View *v = new View("PD_RESULT", Messages::PD_RESULT);
  	v->setDestAddr(ID);
    v->setSrcAddr(myNetwAddr);
//    v->setLength(headerLength);
    v->setKnown(*k);
	sendUp(v);
	EV << "END: " << k->toString().data() << "\n";
	timer = false;
	end = true;
	TRACE->addPD();
	TRACE->addPDNodes(nd->size());
}
