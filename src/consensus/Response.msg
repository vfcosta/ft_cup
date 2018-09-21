cplusplus {{
#include <NetwPkt_m.h>
#include <NeighborDetector.h>
}}


class NetwPkt;

class noncobject NeighborDetector;


message Response extends NetwPkt
{
    int value;
    NeighborDetector dest;
}
