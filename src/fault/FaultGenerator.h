#ifndef FAULTGENERATOR_H_
#define FAULTGENERATOR_H_

#include <omnetpp.h>
#include "util/NeighborDetector.h"

class NeighborDetector;

class FaultGenerator : public cSimpleModule
{
  private:
	int nodes;
	int f;
	NeighborDetector *faults;
	void generate();
	
  public:
    FaultGenerator();
  	bool isFault(int);

  protected:
    virtual void initialize();
    virtual void finish();
};


#endif /*FAULTGENERATOR_H_*/
