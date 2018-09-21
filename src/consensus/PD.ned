package consensus;

simple PD
{
    parameters:
    	bool debug; // debug switch
    	double headerLength; // length of the network packet header (in bits)
    	bool twoHop;
	
    gates:
    	input uppergateIn;
    	output uppergateOut;
    	output upperControlOut;
    	
    	input lowergateIn;
    	input lowerControlIn;
    	output lowergateOut;
}