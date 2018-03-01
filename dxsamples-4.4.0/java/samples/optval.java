//
// Applet file generated automatically 
// time: Fri Dec  2 16:14:28 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class optval extends DXLinkApplication {

    public void readNetwork() {


        SequencerNode Sequencer_1 = new SequencerNode (this.network, "Sequencer", 1, "Sequencer");
        this.network.addElement((Object)Sequencer_1);
        Sequencer_1.setInputValueString(4, "1");
        Sequencer_1.setInputValueString(5, "300");
        Sequencer_1.setInputValueString(6, "10");


        PickNode Pick_1 = new PickNode (this.network, "Pick", 1, "Pick_1");
        this.network.addElement((Object)Pick_1);


        DXLOutputNode DXLOutput_1 = new DXLOutputNode (this.network, "DXLOutput", 1, "DXLOutput_1");
        this.network.addElement((Object)DXLOutput_1);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[49 79.5 21.5292]");
        Image_1.setInputValueString(6, "[-42.0643 211.057 82.5288]");
        Image_1.setInputValueString(10, "[0.214213 -0.284935 0.934305]");
        Image_1.setInputValueString(11, "30.0");
        Image_1.setInputValueString(41, "pick");
        Image_1.setInputValueString(42, "Value");
        Image_1.addInputArc (49, WebOptions_2, 1);
    }
}
