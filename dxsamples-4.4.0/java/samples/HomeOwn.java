//
// Applet file generated automatically 
// time: Fri Dec  2 10:23:31 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class HomeOwn extends DXLinkApplication {

    public void readNetwork() {


        SequencerNode Sequencer_3 = new SequencerNode (this.network, "Sequencer", 3, "Sequencer");
        this.network.addElement((Object)Sequencer_3);
        Sequencer_3.setInputValueString(4, "0");
        Sequencer_3.setInputValueString(5, "9");
        Sequencer_3.setInputValueString(6, "1");


        DrivenNode Colormap_1 = new DrivenNode (this.network, "Colormap", 1, "Colormap");
        this.network.addElement((Object)Colormap_1);


        WebOptionsNode WebOptions_1 = new WebOptionsNode (this.network, "WebOptions", 1, "WebOptions");
        this.network.addElement((Object)WebOptions_1);
        WebOptions_1.setInputValueString(10, "own");


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[2.30354 1.37522 4.0747]");
        Image_1.setInputValueString(6, "[53.0348 -82.6115 99.2387]");
        Image_1.setInputValueString(7, "48.6809");
        Image_1.setInputValueString(10, "[0.106302 0.772951 0.625497]");
        Image_1.setInputValueString(41, "none");
        Image_1.setInputValueString(42, "Ownership");
        Image_1.addInputArc (49, WebOptions_1, 1);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);
        WebOptions_2.setInputValueString(10, "own");


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[0.981204 1.67591 5.045]");
        Image_2.setInputValueString(6, "[51.7125 -82.3108 100.209]");
        Image_2.setInputValueString(7, "50.7831");
        Image_2.setInputValueString(10, "[0.106302 0.772951 0.625497]");
        Image_2.setInputValueString(41, "none");
        Image_2.addInputArc (49, WebOptions_2, 1);


        WebOptionsNode WebOptions_3 = new WebOptionsNode (this.network, "WebOptions", 3, "WebOptions");
        this.network.addElement((Object)WebOptions_3);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(5, "[0.981204 1.67591 5.045]");
        Image_3.setInputValueString(6, "[51.7125 -82.3108 100.209]");
        Image_3.setInputValueString(7, "50.7831");
        Image_3.setInputValueString(10, "[0.106302 0.772951 0.625497]");
        Image_3.setInputValueString(41, "none");
        Image_3.addInputArc (49, WebOptions_3, 1);
    }
}
