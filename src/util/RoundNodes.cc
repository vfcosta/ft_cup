#include "RoundNodes.h"


NeighborDetector *RoundNodes::get(int r) {
	NeighborDetector *nd = NULL;
	std::list<NeighborDetector *>::iterator it;
	for(it=buffer.begin(); it!=buffer.end(); it++) {
		NeighborDetector *n = *it;
		if(n->getRound()==r) {
			nd = n;
			break;
		}
	}

	if(nd==NULL) {
		nd = new NeighborDetector();
		nd->setRound(r);
		buffer.push_back(nd);
	}
	return nd;
}
