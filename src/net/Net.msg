cplusplus {{
#include <NetwPkt_m.h>
#include <NeighborDetector.h>
}};

class NetwPkt;
class noncobject NeighborDetector;

message Net extends NetwPkt {
    fields:
		int initiator;
		NeighborDetector known;
		NeighborDetector dest;
}
