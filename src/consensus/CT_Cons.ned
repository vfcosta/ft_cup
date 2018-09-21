package consensus;

simple CT_Cons
{
    parameters:
    	bool debug = default(false); // debug switch
    	double headerLength; // length of the network packet header (in bits)

    gates:
    	input uppergateIn;
    	output uppergateOut;
    	output upperControlOut;
    	
    	input lowergateIn;
    	input lowerControlIn;
    	output lowergateOut;
}
