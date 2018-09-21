cplusplus {{
#include <NetwPkt_m.h>
#include "NeighborDetector.h"
}}


class NetwPkt;

class noncobject NeighborDetector;


message SinkRequest extends NetwPkt
{
    NeighborDetector known;
}
