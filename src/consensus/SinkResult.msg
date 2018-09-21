cplusplus {{
#include <NetwPkt_m.h>
#include "NeighborDetector.h"
}}


class NetwPkt;

class noncobject NeighborDetector;


message SinkResult extends NetwPkt
{
    bool inSink;
    NeighborDetector known;
}
