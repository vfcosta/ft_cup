package consensus;

import consensus.Consensus;
import consensus.Sink;
import consensus.Collect;
import consensus.PD;
import consensus.FD;

module Cons
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
        consensus: Consensus {
            @display("p=90,50;b=100,20,rect");
        }

        fd: FD {
            @display("p=90,100;b=100,20,rect");
        }
            
        sink: Sink {
            @display("p=90,150;b=100,20,rect");
        }

        collect: Collect {
            @display("p=90,200;b=100,20,rect");
        }

        pd: PD {
            @display("p=90,250;b=100,20,rect");
        }
		
    connections:
        uppergateOut <-- consensus.uppergateOut;
        uppergateIn --> consensus.uppergateIn;
		upperControlOut <-- consensus.upperControlOut;	

        lowergateOut <-- pd.lowergateOut;
        lowergateIn --> pd.lowergateIn;
		lowerControlIn --> pd.lowerControlIn;	

		pd.uppergateOut --> collect.lowergateIn;
		pd.uppergateIn <-- collect.lowergateOut;
		pd.upperControlOut --> collect.lowerControlIn;
		
		collect.uppergateOut --> sink.lowergateIn;
		collect.uppergateIn <-- sink.lowergateOut;
		collect.upperControlOut --> sink.lowerControlIn;

		sink.uppergateOut --> fd.lowergateIn;
		sink.uppergateIn <-- fd.lowergateOut;
		sink.upperControlOut --> fd.lowerControlIn;
		
		fd.uppergateOut --> consensus.lowergateIn;
		fd.uppergateIn <-- consensus.lowergateOut;
		fd.upperControlOut --> consensus.lowerControlIn;

}

