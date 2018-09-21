package app;

simple Application 
{
    parameters:
    	bool debug; // debug switch
    	double headerLength; // length of the application message header (in bits)
    	double K;
    	double F;

    gates:
    	input lowergateIn;
    	input lowerControlIn;
    	output lowergateOut;
}
