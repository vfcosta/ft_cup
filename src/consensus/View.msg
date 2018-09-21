cplusplus {{
#include <NetwPkt_m.h>
#include <NeighborDetector.h>
}}


class NetwPkt;

class noncobject NeighborDetector;


message View extends NetwPkt
{
    int initiator;
    NeighborDetector known;
    NeighborDetector dest;
}
