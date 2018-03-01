//
// Applet file generated automatically 
// time: Fri Dec  2 16:24:13 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class wacc extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Ratios", 1);
        this.network.addPanel(cp_1);



        PickNode Pick_1 = new PickNode (this.network, "Pick", 1, "Pick_1");
        this.network.addElement((Object)Pick_1);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(1);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Corporate Tax Rate");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 0,12,216,52);
        cp_1.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,0,1,0.02,3,0.46);


        ScalarNode Scalar_2 = new ScalarNode (this.network, "Scalar", 2, "Scalar");
        this.network.addElement((Object)Scalar_2);

        StepperInteractor step_Scalar_2 = new StepperInteractor(1);
        Scalar_2.addInteractor(step_Scalar_2);
        step_Scalar_2.setStyle(1);
        step_Scalar_2.setLabelLines(1);
        step_Scalar_2.setLabel("Debt-Equity Ratio");
        step_Scalar_2.setVertical();
        Scalar_2.setBounds (step_Scalar_2, 0,82,216,52);
        cp_1.addInteractor(step_Scalar_2);
        Scalar_2.setValues(1,0,10000,0.1,2,1);


        DrivenNode Colormap_1 = new DrivenNode (this.network, "Colormap", 1, "Colormap");
        this.network.addElement((Object)Colormap_1);


        WebOptionsNode WebOptions_3 = new WebOptionsNode (this.network, "WebOptions", 3, "WebOptions");
        this.network.addElement((Object)WebOptions_3);


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[0.169758 0.158601 0.167673]");
        Image_1.setInputValueString(6, "[-0.870099 -0.707955 0.807327]");
        Image_1.setInputValueString(7, "0.639414");
        Image_1.setInputValueString(10, "[0.308063 0.297393 0.90369]");
        Image_1.setInputValueString(41, "pick");
        Image_1.setInputValueString(42, "WACC");
        Image_1.addInputArc (49, WebOptions_3, 1);
    }
}
