package consensus;

//import	"CT_Cons";
import consensus.ConsensusDecider;
import consensus.GenericConsensus;
import consensus.LambdaRandom;
import consensus.LambdaFD;
import consensus.LambdaLeader;
//import	"FD";
import consensus.LeaderUP;
import consensus.FDManet;

module Consensus 
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

	submodules:
		consdec: ConsensusDecider {
            @display("p=90,50;b=100,20,rect");
		}
	    consensus: GenericConsensus {
            @display("p=90,100;b=100,20,rect");
	    }
	    lambda: LambdaRandom {	//LambdaLeader; LambdaFD;     
            @display("p=90,150;b=100,20,rect");
	    }
	
        fd: LeaderUP { //FDManet FD
            @display("p=90,200;b=100,20,rect");
        }

    connections:
		lowergateOut <-- fd.lowergateOut;
		lowergateIn --> fd.lowergateIn;
		lowerControlIn --> fd.lowerControlIn;
		
		fd.uppergateOut --> lambda.lowergateIn;
		fd.uppergateIn <-- lambda.lowergateOut;
		fd.upperControlOut --> lambda.lowerControlIn;

		lambda.uppergateOut --> consensus.lowergateIn;
		lambda.uppergateIn <-- consensus.lowergateOut;
		lambda.upperControlOut --> consensus.lowerControlIn;
		
		consensus.uppergateOut --> consdec.lowergateIn;
		consensus.uppergateIn <-- consdec.lowergateOut;
		consensus.upperControlOut --> consdec.lowerControlIn;
		
		uppergateOut <-- consdec.uppergateOut;
        uppergateIn --> consdec.uppergateIn;
		upperControlOut <-- consdec.upperControlOut;	

}
