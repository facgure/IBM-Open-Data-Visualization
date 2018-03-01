//
// Applet file generated automatically 
// time: Fri Dec  2 14:48:02 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class ddi extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Data Selection", 1);
        this.network.addPanel(cp_1);



        SelectionNode Selector_4 = new SelectionNode (this.network, "Selector", 4, "Selector");
        this.network.addElement((Object)Selector_4);

        Selector pdown_Selector_4 = new Selector();
        Selector_4.addInteractor(pdown_Selector_4);
        pdown_Selector_4.setStyle(1);
        pdown_Selector_4.setLabelLines(1);
        pdown_Selector_4.setLabel("data variable");
        pdown_Selector_4.setHorizontal();
        Selector_4.setBounds (pdown_Selector_4, 0,0,262,38);
        pdown_Selector_4.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(pdown_Selector_4);
        Vector Selector_4_vn = new Vector(4);
        Vector Selector_4_vo = new Vector(4);
        Selector_4_vn.addElement("wind");
        Selector_4_vo.addElement("1");
        Selector_4_vn.addElement("temperature");
        Selector_4_vo.addElement("2");
        Selector_4_vn.addElement("cloudwater");
        Selector_4_vo.addElement("3");
        Selector_4_vn.addElement("rainwater");
        Selector_4_vo.addElement("4");
        Selector_4.setValues(Selector_4_vn, Selector_4_vo);
        Selector_4.selectOption(4);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(1);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Isosurface value:");
        step_Scalar_1.setHorizontal();
        Scalar_1.setBounds (step_Scalar_1, 0,120,262,36);
        cp_1.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,-2.01298e-06,11.5107,1,1,1);


        ScalarNode Integer_1 = new ScalarNode (this.network, "Integer", 1, "Integer");
        this.network.addElement((Object)Integer_1);

        StepperInteractor step_Integer_1 = new StepperInteractor(1);
        Integer_1.addInteractor(step_Integer_1);
        step_Integer_1.setStyle(1);
        step_Integer_1.setLabelLines(1);
        step_Integer_1.setLabel("Slab position:");
        step_Integer_1.setHorizontal();
        Integer_1.setBounds (step_Integer_1, 0,60,262,36);
        cp_1.addInteractor(step_Integer_1);
        Integer_1.setValues(1,0,7,1,2);


        DrivenNode Colormap_4 = new DrivenNode (this.network, "Colormap", 4, "Colormap");
        this.network.addElement((Object)Colormap_4);


        DXLOutputNode DXLOutput_2 = new DXLOutputNode (this.network, "DXLOutput", 2, "DXLOutput_2");
        this.network.addElement((Object)DXLOutput_2);


        WebOptionsNode WebOptions_1 = new WebOptionsNode (this.network, "WebOptions", 1, "WebOptions");
        this.network.addElement((Object)WebOptions_1);
        WebOptions_1.setInputValueString(1, "jpg");


        ImageNode Image_5 = new ImageNode (this.network, "Image", 5, "Image");
        this.network.addElement((Object)Image_5);
        Image_5.setInputValueString(5, "[50000 7750 27000]");
        Image_5.setInputValueString(6, "[-102667 130295 238549]");
        Image_5.setInputValueString(7, "116712.0");
        Image_5.setInputValueString(10, "[0.103227 0.891182 -0.441745]");
        Image_5.setInputValueString(41, "none");
        Image_5.setInputValueString(42, "Isosurface");
        Image_5.addInputArc (49, WebOptions_1, 1);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);
        WebOptions_2.setInputValueString(1, "gif");


        ImageNode Image_7 = new ImageNode (this.network, "Image", 7, "Image");
        this.network.addElement((Object)Image_7);
        Image_7.setInputValueString(10, "[0 1 0]");
        Image_7.addInputArc (49, WebOptions_2, 1);
    }
}
