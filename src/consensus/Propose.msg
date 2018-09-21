cplusplus {{
#include <NetwPkt_m.h>
#include "NeighborDetector.h"
}}


class NetwPkt;

class noncobject NeighborDetector;


message Propose extends NetwPkt
{
    int value;
    int round;
    int leader;
    NeighborDetector dest;
}
