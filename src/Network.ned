import Host;
import org.mf2.core.channelControl.ChannelControl;
import trace.Trace;
import fault.FaultGenerator;


module Sim {
    parameters:
        // parametes for the framework
        double playgroundSizeX;
		double playgroundSizeY;
        int numHosts;
        

    submodules:
    	trace: Trace {
    		@display("p=470,20;rect");
    	}

    	faultGenerator: FaultGenerator {
    		@display("p=470,65;rect");
    	}
    	
        channelcontrol: ChannelControl {
            parameters:
                playgroundSizeX = playgroundSizeX;
				playgroundSizeY = playgroundSizeY;
            	@display("p=50,25;i=eye14");
        }
        
        host[numHosts]: Host {
            @display("p=50,50;i=node");
        }

    connections allowunconnected:
    // all connections and gates are to be generated dynamically

//    @display("p=0,0;b=$playgroundSizeX,$playgroundSizeY,rect;o=white");
}

network sim extends Sim {
    parameters:
        playgroundSizeX @prompt("playgroundSizeX") = default(40);
        playgroundSizeY @prompt("playgroundSizeY") = default(40);
        numHosts @prompt("Number of hosts:") = default(2);
}
