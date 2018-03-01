//
// Applet file generated automatically 
// time: Fri Dec  2 14:33:08 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

//
// These are required in order to access the TouchSensor
// and ProximitySensor in cosmo.  They're included here
// because a WebOptions macro has its format input set to "wrl"
//
public class ThunderGlyphSheet extends WRLApplication  {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Controls", 1);
        this.network.addPanel(cp_1);



        ScalarNode Scalar_6 = new ScalarNode (this.network, "Scalar", 6, "Scalar");
        this.network.addElement((Object)Scalar_6);

        StepperInteractor step_Scalar_6 = new StepperInteractor(1);
        Scalar_6.addInteractor(step_Scalar_6);
        step_Scalar_6.setStyle(1);
        step_Scalar_6.setLabelLines(1);
        step_Scalar_6.setLabel("vertical position of maptoplane");
        step_Scalar_6.setVertical();
        Scalar_6.setBounds (step_Scalar_6, 0,72,197,52);
        cp_1.addInteractor(step_Scalar_6);
        Scalar_6.setValues(1,0,15000,500,0,3000);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(1);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Isovalue");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 209,0,197,52);
        cp_1.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,0.1,2,0.01,2,0.2);


        ScalarNode Scalar_7 = new ScalarNode (this.network, "Scalar", 7, "Scalar");
        this.network.addElement((Object)Scalar_7);

        StepperInteractor step_Scalar_7 = new StepperInteractor(1);
        Scalar_7.addInteractor(step_Scalar_7);
        step_Scalar_7.setStyle(1);
        step_Scalar_7.setLabelLines(1);
        step_Scalar_7.setLabel("isosurface opacity");
        step_Scalar_7.setVertical();
        Scalar_7.setBounds (step_Scalar_7, 209,72,197,52);
        cp_1.addInteractor(step_Scalar_7);
        Scalar_7.setValues(1,0,1,0.017,5,0.306);


        ScalarNode Scalar_2 = new ScalarNode (this.network, "Scalar", 2, "Scalar");
        this.network.addElement((Object)Scalar_2);

        StepperInteractor step_Scalar_2 = new StepperInteractor(1);
        Scalar_2.addInteractor(step_Scalar_2);
        step_Scalar_2.setStyle(1);
        step_Scalar_2.setLabelLines(1);
        step_Scalar_2.setLabel("RubberSheet scale:");
        step_Scalar_2.setVertical();
        Scalar_2.setBounds (step_Scalar_2, 0,0,197,52);
        cp_1.addInteractor(step_Scalar_2);
        Scalar_2.setValues(1,-1000,1000,10,0,430);


        WebOptionsNode WebOptions_6 = new WebOptionsNode (this.network, "WebOptions", 6, "WebOptions");
        this.network.addElement((Object)WebOptions_6);
        WebOptions_6.setInputValueString(1, "wrl");


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[33663.4 2919.17 27329]");
        Image_2.setInputValueString(6, "[128484 77531.6 287705]");
        Image_2.setInputValueString(7, "52096.1");
        Image_2.setInputValueString(10, "[-0.0150042 0.962635 -0.270385]");
        Image_2.setInputValueString(41, "none");
        Image_2.addInputArc (49, WebOptions_6, 1);


        WebOptionsNode WebOptions_7 = new WebOptionsNode (this.network, "WebOptions", 7, "WebOptions");
        this.network.addElement((Object)WebOptions_7);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(5, "[33663.4 2919.17 27329]");
        Image_3.setInputValueString(6, "[128484 77531.6 287705]");
        Image_3.setInputValueString(7, "52096.1");
        Image_3.setInputValueString(10, "[-0.0150042 0.962635 -0.270385]");
        Image_3.setInputValueString(41, "none");
        Image_3.addInputArc (49, WebOptions_7, 1);
    }
}
