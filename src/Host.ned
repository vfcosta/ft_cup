import app.Application;
import consensus.FT_CUP;
import net.FloodRouter;
import net.WuRouter;
import org.mf2.contrib.nic.Nic80211;
import org.mf2.contrib.nic.CoreTestNic;
import org.mf2.contrib.nic.AlohaSnrNic;
import org.mf2.contrib.nic.P2PNic;
import org.mf2.contrib.netwLayer.SimpleArp;
import org.mf2.core.basicModules.BasicMobility;
import org.mf2.core.blackboard.Blackboard;


module Host
{
    parameters:
        string mobilityType;

    gates:
        input radioIn; // gate for sendDirect

    submodules:
        blackboard: Blackboard {
            @display("p=200,75;b=30,25");
        }

        //mobility: ConstSpeedMobility;
        //	display: "p=200,150;b=30,25";

        mobility: BasicMobility {
            @display("p=200,150;i=block/cogwheel");
        }

        appl: Application {
            @display("p=90,50;b=100,20,rect");
        }

        arp: SimpleArp {
            @display("p=200,225;b=30,25");
        }

        cons: FT_CUP {
            @display("p=90,100;b=100,20,rect");
        }

        net: FloodRouter { //WuRouter; //Dsr; //AODV
            @display("p=90,150;b=100,20,rect");
        }

        nic: Nic80211 { //P2PNic;	//CoreTestNic;	//AlohaSnrNic;	
            @display("p=90,200;b=100,20,rect");
        }

    connections:
        nic.uppergateOut --> net.lowergateIn;
        nic.uppergateIn <-- net.lowergateOut;
        nic.upperControlOut --> net.lowerControlIn;

//		net.uppergateOut --> appl.lowergateIn;
//		net.uppergateIn <-- appl.lowergateOut;
//		net.upperControlOut --> appl.lowerControlIn;		

        net.uppergateOut --> cons.lowergateIn;
        net.uppergateIn <-- cons.lowergateOut;
        net.upperControlOut --> cons.lowerControlIn;

        cons.uppergateOut --> appl.lowergateIn;
        cons.uppergateIn <-- appl.lowergateOut;
        cons.upperControlOut --> appl.lowerControlIn;

        radioIn --> nic.radioIn;
 	    //@display("p=10,10;b=250,250,rect;o=white");
}
