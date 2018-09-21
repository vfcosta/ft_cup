#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

template <class T>
T *__checknull(T *p, const char *expr, const char *file, int line) {
//    if (!p)
//        opp_error("Expression %s returned NULL at %s line %d ", expr, file, line);
    return p;
}
#define CHKNULL(x) __checknull((x), #x, __FILE__, __LINE__)

#include <Trace.h>
#include <FaultGenerator.h>

//constantes para o modulo do consenso (dentro do ft-cup)
#define FAULT check_and_cast<FaultGenerator *> (getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("faultGenerator"))
#define TRACE_CONS check_and_cast<Trace *> (getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("trace"))
#define ID_CONS getParentModule()->getParentModule()->getParentModule()->getIndex()
#define KN_CONS ((int) getParentModule()->getParentModule()->getParentModule()->getSubmodule("appl")->par("K"))
#define F_CONS ((int) getParentModule()->getParentModule()->getParentModule()->getSubmodule("appl")->par("F"))
//


#define ID getParentModule()->getParentModule()->getIndex()
//#define KN 1
#define KN ((int) getParentModule()->getParentModule()->getSubmodule("appl")->par("K"))
#define F ((int) getParentModule()->getParentModule()->getSubmodule("appl")->par("F"))
#define NIL -1
#define TRACE check_and_cast<Trace *> (getParentModule()->getParentModule()->getParentModule()->getSubmodule("trace"))
#define FAULTAPP check_and_cast<FaultGenerator *> (getParentModule()->getParentModule()->getSubmodule("faultGenerator"))

class Messages {

	public:
  	enum NET_MSG_TYPES{
		HELLO,
		HELLO_RESPONSE,
		SEND_TIMER,
		VIEW,
		INIT_PD,
		INIT_COLLECT,
		INIT_SINK,
		PROPOSE,
		SINK_REQUEST,
		SINK_RESPONSE,
		COLLECT_RESULT,
		PD_RESULT,
		SINK_RESULT,
		REQUEST,
		RESPONSE,
		CONSENSUS_RESULT,
		PHASE1,
		PHASE2,
		DECISION,
		I_AM_ALIVE,
		FD_TIMEOUT,
		FD_RESULT,
		INIT_FD,
		END_FD,
		FD_QUERY,
		FD_RESPONSE,
		INIT_LAMBDA,
		END_LAMBDA,
		LAMBDA_RESULT,
		AODV_RREQ,
		AODV_RREP,
		NET_HELLO,
		NET
	};
};

#endif /*CONFIGURATION_H_*/
