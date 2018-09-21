#ifndef ROUND_NODES_H
#define ROUND_NODES_H

#include <list>

#include "NeighborDetector.h"
#include <Configuration.h>

#include <string>

class Node;

class RoundNodes
{
  public:
	RoundNodes() {buffer = std::list<NeighborDetector *>();}

	NeighborDetector *get(int);
  private:
	std::list<NeighborDetector *> buffer;
};

#endif
