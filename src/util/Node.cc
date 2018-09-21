/**
 * Representa um no da rede.
 *
 */

#include "Node.h"
#include "NeighborDetector.h"

Node::Node() {
}

Node::Node(int id_) {
	Node(id_, 0);
}

Node::Node(int id_, double time_) {
	id = id_;
	time = time_;
	counter = 0;
	parent = -1;

	neighbors = new NeighborDetector();
}

int Node::getId()
{
	return id;
}

double Node::getTime()
{
	return time;
}


void Node::setId(int id_)
{
	id = id_;
}

void Node::setTime(double time_)
{
	time = time_;
}

bool Node::operator==(const Node &n) const
{
	return id == n.id;
}



void Node::setValue(int value_)
{
	value = value_;
}


int Node::getValue()
{
	return value;
}

int Node::getCounter() {
	return counter;
}

void Node::setCounter(int c) {
	counter = c;
}

int Node::getParent() {
	return parent;
}

void Node::setParent(int p) {
	parent = p;
}
