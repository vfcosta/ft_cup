#include "NeighborDetector.h"

#include <NetwControlInfo.h>
#include <SimpleAddress.h>
#include <algorithm>

NeighborDetector::NeighborDetector() {
}

void NeighborDetector::add(int src, double time)
{
	Node n = Node(src, time);
	n.setId(src);
	buffer[n.getId()] = n;
}

void NeighborDetector::add(int src)
{
	Node n = Node(src, 0);
	n.setId(src);
	buffer[n.getId()] = n;
}

void NeighborDetector::add(Node n)
{
	buffer[n.getId()] = n;
}

void NeighborDetector::add(NeighborDetector nd)
{
	std::map<int, Node> buf = nd.getBuffer();
	std::map<int, Node>::iterator it;
	for(it=buf.begin(); it!=buf.end(); it++)
	{
		this->buffer[it->first] = it->second;
	}
}

Node NeighborDetector::get(int id)
{
	std::map<int, Node>::iterator it = buffer.find( id );
	if( it != buffer.end() )
	{
		return it->second;
	}
	Node n = Node();
	n.setId(-1);
	n.setValue(-1);
	n.setCounter(-1);
	return n;
}


int NeighborDetector::size() {
	return buffer.size();
}


std::map<int,Node> NeighborDetector::intersection(std::map<int,Node> d) {
	std::map<int,Node> tmp;
	set_intersection(buffer.begin(), buffer.end(), d.begin(), d.end(), inserter(tmp, tmp.begin()), buffer.value_comp());
	return tmp;
}

std::map<int,Node> NeighborDetector::difference(std::map<int,Node> d) {
	std::map<int,Node> tmp;
	set_difference(buffer.begin(), buffer.end(), d.begin(), d.end(), inserter(tmp, tmp.begin()), buffer.value_comp());
	return tmp;
}

std::map<int,Node> NeighborDetector::unions(std::map<int,Node> d) {
	std::map<int,Node> tmp;
	set_union(buffer.begin(), buffer.end(), d.begin(), d.end(), inserter(tmp, tmp.begin()), buffer.value_comp());
	return tmp;
}

std::map<int,Node> NeighborDetector::getBuffer() {
	return buffer;
}

void NeighborDetector::setBuffer(std::map<int,Node> s) {
	buffer = s;
}


std::string NeighborDetector::toString() {
	std::string ret = "[";
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		Node n = it->second;
		char buf[20];
		sprintf(buf, "%d", n.getId());
		if(it != buffer.begin()) ret.append(", ");
		ret.append(buf);
	}
	return ret.append("]");
}

void NeighborDetector::copyFrom(NeighborDetector n) {
	buffer = std::map<int, Node>(n.getBuffer());
}


bool NeighborDetector::equals(NeighborDetector n) {
	return buffer == n.getBuffer();
}

void NeighborDetector::clear() {
	buffer.clear();
}

void NeighborDetector::remove(NeighborDetector nd) {
	std::map<int,Node> d = nd.getBuffer();
	std::map<int,Node> tmp;
	set_difference(buffer.begin(), buffer.end(), d.begin(), d.end(), inserter(tmp, tmp.begin()), buffer.value_comp());
	buffer = std::map<int,Node>(tmp);
}

void NeighborDetector::remove(int src)
{
	buffer.erase(src);
}

bool NeighborDetector::empty() {
	return buffer.empty();
}

int NeighborDetector::getValue() {
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		Node n = it->second;
		if(n.getValue()!=NIL) return n.getValue();
	}
	return NIL;
}

//verifica se todos possuem o mesmo valor v
bool NeighborDetector::values(int v) {
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		Node n = it->second;
		if(n.getValue()!=v) return false;
	}
	return true;
}


bool NeighborDetector::contains(int n) {
	return get(n).getId() != -1;
}


Node NeighborDetector::index(int i) {
	int count = 0;
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		Node n = it->second;
		if(i==count) return n;
		count++;
	}
	return Node(-1);
}

NeighborDetector *NeighborDetector::copy() {
	NeighborDetector *ret = new NeighborDetector();
	ret->copyFrom(*this);
	return ret;
}

int NeighborDetector::getMajorityValue() {
	std::map<int, int> values;
	std::map<int, int>::iterator itv;
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		Node n = it->second;
		itv = values.find( n.getValue() );
		if( itv != values.end() ) {
			values[n.getValue()]++;
		}
		else {
			values[n.getValue()] = 1;
		}
	}

	for(itv = values.begin(); itv != values.end(); itv++)
	{
		int value = itv->first;
		int count = itv->second;
		if(count > buffer.size()/2) {
			return value;
		}
	}

	return NIL;
}

int NeighborDetector::getMajorityCounter() {
	std::map<int, int> values;
	std::map<int, int>::iterator itv;
	std::map<int, Node>::iterator it;
	for(it = buffer.begin(); it != buffer.end(); it++)
	{
		Node n = it->second;
		itv = values.find( n.getCounter() );
		if( itv != values.end() ) {
			values[n.getCounter()]++;
		}
		else {
			values[n.getCounter()] = 1;
		}
	}

	for(itv = values.begin(); itv != values.end(); itv++)
	{
		int value = itv->first;
		int count = itv->second;
		if(count > buffer.size()/2) {
			return value;
		}
	}

	return NIL;
}

void NeighborDetector::addAll(std::map<int,Node> d) {
	std::map<int,Node> tmp;
	set_union(buffer.begin(), buffer.end(), d.begin(), d.end(), inserter(tmp, tmp.begin()), buffer.value_comp());
	buffer = tmp;
}

bool NeighborDetector::operator==(const NeighborDetector &n) const
{
	return round == n.round;
}
