package consensus;

simple GenericConsensus
{
    parameters:
    	bool debug; // debug switch
    	double headerLength; // length of the network packet header (in bits)

    gates:
    	input uppergateIn;
    	output uppergateOut;
    	output upperControlOut;
    	
    	input lowergateIn;
    	input lowerControlIn;
    	output lowergateOut;
}