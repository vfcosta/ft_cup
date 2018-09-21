#include "Application.h"

#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include "../consensus/PD.h"
#include "../consensus/Response_m.h"

#define RESCHEDULE simTime() + 1//dblrand()

Define_Module(Application);


// do some initialization
void Application::initialize(int stage)
{
    BasicApplLayer::initialize(stage);
    if(stage == 0) {
        endToEndDelayVec.setName("End-to-End Delay");
        delayTimer = new cMessage( "delay-timer", Messages::SEND_TIMER );
        /**
	  	* invocando submodulo Trace
	  	*/
        cModule *simMod = getParentModule()->getParentModule();
        cModule *mod = simMod->getSubmodule("trace");
   		trace = check_and_cast<Trace *>(mod);

        if(hasPar("K")) {
        	Kn = par("K");
        }
        else {
        	Kn = 1;
			par("K") = Kn;
        }
        if(hasPar("F")) {
        	Fn = par("F");
        }
        else {
        	Fn = Kn-1;
//			addPar("F")->setLongValue(Fn);
        	par("F") = Fn;
        }
    }
    else if(stage==1) {
   		scheduleAt(RESCHEDULE, delayTimer);
		EV << "fault: "<<FAULTAPP->isFault(myApplAddr()) << endl;
    }
}

// You have send yourself a message -- probably a timer,
// take care of it
void Application::handleSelfMsg(cMessage *msg)
{
    switch( msg->getKind() ){
    case Messages::SEND_TIMER:
        propose();
		cancelAndDelete(msg);
		break;
    default:
    	cancelAndDelete(msg);
    }
}

void Application::propose() {
	ApplPkt *pkt = new ApplPkt("PROPOSE", Messages::PROPOSE);
    pkt->setDestAddr(-1);
    // we use the host modules index() as a appl address
    pkt->setSrcAddr( myApplAddr() );
//    pkt->setLength(headerLength);

    EV << "Sending init pd packet to network layer!\n";

	if(!FAULTAPP->isFault(myApplAddr()))
		sendDown( pkt );
}

// You got a control message from the network layer
// take care of it
void Application::handleLowerControl( cMessage *msg )
{
    // ENTER YOUR CODE HERE
    delete msg;
}

void Application::finish()
{
    BasicApplLayer::finish();
    if(!delayTimer->isScheduled()) delete delayTimer;
}

// You got a message from the network layer (most likely)
// take care of it
void Application::handleLowerMsg( cMessage *msg )
{
	NetwPkt *m = static_cast<NetwPkt *>(msg);

	if(m->getDestAddr()!=ID && m->getDestAddr()!=L3BROADCAST) {
		delete msg;
		return;
	}

    switch( msg->getKind() ){
	case Messages::CONSENSUS_RESULT: {
		//FIM do consenso!!
		Response *v = static_cast<Response *>(m);
		delete msg;
	}
    break;
    default:
		delete msg;
    }
}
