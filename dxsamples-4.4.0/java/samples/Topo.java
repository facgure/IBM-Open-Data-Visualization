//
// Applet file generated automatically 
// time: Fri Dec  2 14:39:30 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class Topo extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Options", 1);
        this.network.addPanel(cp_1);



        ScalarNode Integer_1 = new ScalarNode (this.network, "Integer", 1, "Integer");
        this.network.addElement((Object)Integer_1);

        StepperInteractor step_Integer_1 = new StepperInteractor(1);
        Integer_1.addInteractor(step_Integer_1);
        step_Integer_1.setStyle(1);
        step_Integer_1.setLabelLines(1);
        step_Integer_1.setLabel("reduction exponent");
        step_Integer_1.setVertical();
        Integer_1.setBounds (step_Integer_1, 0,7,226,52);
        cp_1.addInteractor(step_Integer_1);
        Integer_1.setValues(1,0,3,1,1);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(1);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("rubber sheet scale factor");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 0,90,226,52);
        cp_1.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,0,1,0.1,2,0.5);


        WebOptionsNode WebOptions_1 = new WebOptionsNode (this.network, "WebOptions", 1, "WebOptions");
        this.network.addElement((Object)WebOptions_1);


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[196.368 193.184 57.397]");
        Image_2.setInputValueString(6, "[625.786 -755.708 806.556]");
        Image_2.setInputValueString(7, "344.652");
        Image_2.setInputValueString(10, "[-0.0583675 0.6022 0.796209]");
        Image_2.setInputValueString(41, "rotate");
        Image_2.setInputValueString(42, "Colorado");
        Image_2.addInputArc (49, WebOptions_1, 1);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);


        ImageNode Image_4 = new ImageNode (this.network, "Image", 4, "Image");
        this.network.addElement((Object)Image_4);
        Image_4.setInputValueString(5, "[196.368 193.184 57.397]");
        Image_4.setInputValueString(6, "[625.786 -755.708 806.556]");
        Image_4.setInputValueString(7, "344.652");
        Image_4.setInputValueString(10, "[-0.0583675 0.6022 0.796209]");
        Image_4.addInputArc (49, WebOptions_2, 1);
    }
}
