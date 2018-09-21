cplusplus {{
#include <NetwPkt_m.h>
#include <NeighborDetector.h>
}};

class NetwPkt;
class noncobject NeighborDetector;

message FDManetQR extends NetwPkt {
    fields:
		NeighborDetector received;
		NeighborDetector suspected;
		NeighborDetector mistake;
}
