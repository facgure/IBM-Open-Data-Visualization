//
// Applet file generated automatically 
// time: Fri Dec  2 10:31:43 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class MRI_2 extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_2 = new ControlPanel(this.network, null, 2);
        this.network.addPanel(cp_2);



        SequencerNode Sequencer_1 = new SequencerNode (this.network, "Sequencer", 1, "Sequencer");
        this.network.addElement((Object)Sequencer_1);
        Sequencer_1.setInputValueString(4, "0");
        Sequencer_1.setInputValueString(5, "15");
        Sequencer_1.setInputValueString(6, "1");


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(1);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Isosurface value:");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 20,20,130,54);
        cp_2.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,668,57684,1000,1,39000);


        WebOptionsNode WebOptions_3 = new WebOptionsNode (this.network, "WebOptions", 3, "WebOptions");
        this.network.addElement((Object)WebOptions_3);


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[113.724 112.595 31.5]");
        Image_1.setInputValueString(6, "[-238.015 329.363 642.046]");
        Image_1.setInputValueString(7, "265.729");
        Image_1.setInputValueString(10, "[0.0757243 0.952618 -0.294592]");
        Image_1.setInputValueString(41, "none");
        Image_1.setInputValueString(42, "MRI");
        Image_1.addInputArc (49, WebOptions_3, 1);
    }
}
