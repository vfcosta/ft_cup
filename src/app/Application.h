
#ifndef APPLICATION_H
#define APPLICATION_H

#include <SimpleNetwLayer.h>

#include <BasicApplLayer.h>

#include <NeighborDetector.h>
#include <Configuration.h>

class NeighborDetector;
class Trace;

class Application : public BasicApplLayer
{
  public:
    virtual void initialize(int);
    virtual void finish();

  private:
    cOutVector endToEndDelayVec; //exemplo de log vector
   	Trace *trace;
   	cMessage *delayTimer;
   	int Kn;
   	int Fn;

  protected:
    /** @brief Handle self messages such as timer... */
    virtual void handleSelfMsg(cMessage*);

    /** @brief Handle messages from lower layer */
    virtual void handleLowerMsg(cMessage*);

    /** @brief Handle control messages from lower layer */
    virtual void handleLowerControl(cMessage* msg);

    /** @brief send a hello packet to all connected neighbors */
    void propose();

};

#endif
