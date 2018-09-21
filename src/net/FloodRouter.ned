package net;

simple FloodRouter
{
    parameters:
	bool debug; // debug switch
	double headerLength; // length of the network header
	bool plainFlooding; // flag whether to use plain flooding
	// Max number of entries in the list of already broadcasted messages
	double bcMaxEntries;
	// Time after which an entry for an already broadcasted msg can be deleted
	double bcDelTime;
	double defaultTtl; // Default time-to-live (ttl) used for this module

    gates:
    	input uppergateIn; // from application layer
    	output uppergateOut; // to application layer
    	output upperControlOut; // for control messages to upper layer
    	input lowergateIn; // from MAC layer
    	input lowerControlIn;   // for control messages from mac layer
    	output lowergateOut; // to MAC layer
}