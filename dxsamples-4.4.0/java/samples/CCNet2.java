//
// Applet file generated automatically 
// time: Fri Dec  2 10:15:29 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class CCNet2 extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_2 = new ControlPanel(this.network, "Settings", 2);
        this.network.addPanel(cp_2);



        PickNode Pick_1 = new PickNode (this.network, "Pick", 1, "Pick_1");
        this.network.addElement((Object)Pick_1);


        SelectionNode Selector_3 = new SelectionNode (this.network, "Selector", 3, "Selector");
        this.network.addElement((Object)Selector_3);

        Selector pdown_Selector_3 = new Selector();
        Selector_3.addInteractor(pdown_Selector_3);
        pdown_Selector_3.setStyle(0);
        pdown_Selector_3.setLabelLines(1);
        pdown_Selector_3.setLabel("Select Data Range");
        pdown_Selector_3.setHorizontal();
        Selector_3.setBounds (pdown_Selector_3, 0,96,198,40);
        pdown_Selector_3.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(pdown_Selector_3);
        Vector Selector_3_vn = new Vector(2);
        Vector Selector_3_vo = new Vector(2);
        Selector_3_vn.addElement("on");
        Selector_3_vo.addElement("1");
        Selector_3_vn.addElement("off");
        Selector_3_vo.addElement("2");
        Selector_3.setValues(Selector_3_vn, Selector_3_vo);
        Selector_3.selectOption(1);


        SelectionNode Selector_2 = new SelectionNode (this.network, "Selector", 2, "Selector");
        this.network.addElement((Object)Selector_2);

        Selector pdown_Selector_2 = new Selector();
        Selector_2.addInteractor(pdown_Selector_2);
        pdown_Selector_2.setStyle(0);
        pdown_Selector_2.setLabelLines(1);
        pdown_Selector_2.setLabel("Upper Triangle:");
        pdown_Selector_2.setHorizontal();
        Selector_2.setBounds (pdown_Selector_2, 0,48,198,40);
        pdown_Selector_2.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(pdown_Selector_2);
        Vector Selector_2_vn = new Vector(2);
        Vector Selector_2_vo = new Vector(2);
        Selector_2_vn.addElement("on");
        Selector_2_vo.addElement("1");
        Selector_2_vn.addElement("off");
        Selector_2_vo.addElement("2");
        Selector_2.setValues(Selector_2_vn, Selector_2_vo);
        Selector_2.selectOption(2);


        SelectionNode Selector_1 = new SelectionNode (this.network, "Selector", 1, "Selector");
        this.network.addElement((Object)Selector_1);

        Selector pdown_Selector_1 = new Selector();
        Selector_1.addInteractor(pdown_Selector_1);
        pdown_Selector_1.setStyle(0);
        pdown_Selector_1.setLabelLines(2);
        pdown_Selector_1.setLabel("Diagonal  elemenets ");
        pdown_Selector_1.setLabel("shown as white:");
        pdown_Selector_1.setHorizontal();
        Selector_1.setBounds (pdown_Selector_1, 0,0,204,40);
        pdown_Selector_1.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(pdown_Selector_1);
        Vector Selector_1_vn = new Vector(2);
        Vector Selector_1_vo = new Vector(2);
        Selector_1_vn.addElement("on");
        Selector_1_vo.addElement("2");
        Selector_1_vn.addElement("off");
        Selector_1_vo.addElement("1");
        Selector_1.setValues(Selector_1_vn, Selector_1_vo);
        Selector_1.selectOption(1);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(0);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Min CC");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 204,0,197,54);
        cp_2.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,0,1,0.01,2,0.4);


        DXLOutputNode DXLOutput_1 = new DXLOutputNode (this.network, "DXLOutput", 1, "DXLOutput_1");
        this.network.addElement((Object)DXLOutput_1);


        DXLOutputNode DXLOutput_2 = new DXLOutputNode (this.network, "DXLOutput", 2, "DXLOutput_2");
        this.network.addElement((Object)DXLOutput_2);


        SelectionNode Selector_4 = new SelectionNode (this.network, "Selector", 4, "Selector");
        this.network.addElement((Object)Selector_4);

        Selector pdown_Selector_4 = new Selector();
        Selector_4.addInteractor(pdown_Selector_4);
        pdown_Selector_4.setStyle(0);
        pdown_Selector_4.setLabelLines(2);
        pdown_Selector_4.setLabel("3D representation ");
        pdown_Selector_4.setLabel(" off when select data is off:");
        pdown_Selector_4.setVertical();
        Selector_4.setBounds (pdown_Selector_4, 204,72,197,74);
        pdown_Selector_4.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(pdown_Selector_4);
        Vector Selector_4_vn = new Vector(2);
        Vector Selector_4_vo = new Vector(2);
        Selector_4_vn.addElement("on");
        Selector_4_vo.addElement("1");
        Selector_4_vn.addElement("off");
        Selector_4_vo.addElement("2");
        Selector_4.setValues(Selector_4_vn, Selector_4_vo);
        Selector_4.selectOption(2);


        WebOptionsNode WebOptions_13 = new WebOptionsNode (this.network, "WebOptions", 13, "WebOptions");
        this.network.addElement((Object)WebOptions_13);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(5, "[125.5 -127.5 -0.00964314]");
        Image_3.setInputValueString(6, "[125.5 -127.5 668.995]");
        Image_3.setInputValueString(10, "[0 1 0]");
        Image_3.setInputValueString(11, "9.6494");
        Image_3.setInputValueString(41, "pick");
        Image_3.setInputValueString(42, "Correlation");
        Image_3.addInputArc (49, WebOptions_13, 1);


        WebOptionsNode WebOptions_14 = new WebOptionsNode (this.network, "WebOptions", 14, "WebOptions");
        this.network.addElement((Object)WebOptions_14);


        ImageNode Image_5 = new ImageNode (this.network, "Image", 5, "Image");
        this.network.addElement((Object)Image_5);
        Image_5.setInputValueString(5, "[740 140 0]");
        Image_5.setInputValueString(6, "[800 180 7014.12]");
        Image_5.setInputValueString(7, "2702.36");
        Image_5.setInputValueString(10, "[0 1 0]");
        Image_5.setInputValueString(41, "none");
        Image_5.addInputArc (49, WebOptions_14, 1);
    }
}
