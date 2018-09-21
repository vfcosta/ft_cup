
#ifndef NEIGHBOR_DETECTOR_H
#define NEIGHBOR_DETECTOR_H

#include <map>

//#include <Blackboard.h>
#include "Node.h"
#include <Configuration.h>

#include <string>

class Node;

class NeighborDetector /*: public BBItem*/
{
  public:
    NeighborDetector();
    void add(int, double);
    void add(int);
    void add(Node);
    void add(NeighborDetector);
    void sendHello();
    Node get(int);
    int size();
    std::map<int,Node> getBuffer();
    void setBuffer(std::map<int,Node>);
    std::map<int,Node> difference(std::map<int,Node>);
    std::map<int,Node> unions(std::map<int,Node>);
    std::map<int,Node> intersection(std::map<int,Node>);
    void remove(NeighborDetector);
    void remove(int);
	bool empty();
    std::string toString();
    int getValue();
    void copyFrom(NeighborDetector);
    bool equals(NeighborDetector);
	bool values(int);
    void clear();
	bool contains(int);
	Node index(int);
	NeighborDetector *copy();
	int getMajorityValue();
	int getMajorityCounter();
	void addAll(std::map<int,Node> d);
	void setRound(int r) {this->round=r;};
	int getRound() {return this->round;};
    bool operator==(const NeighborDetector &) const;

  private:
	std::map<int,Node> buffer;
	int round;
};

#endif
