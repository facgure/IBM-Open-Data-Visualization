//
// Applet file generated automatically 
// time: Thu Dec  1 13:51:23 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class GroceryList extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Data Selection", 1);
        this.network.addPanel(cp_1);



        ToggleNode Toggle_1 = new ToggleNode (this.network, "Toggle", 1, "Toggle");
        this.network.addElement((Object)Toggle_1);


        SelectionNode Selector_1 = new SelectionNode (this.network, "Selector", 1, "Selector");
        this.network.addElement((Object)Selector_1);

        RadioGroup pdown_Selector_1 = new RadioGroup();
        Selector_1.addInteractor(pdown_Selector_1);
        pdown_Selector_1.setStyle(1);
        pdown_Selector_1.setLabelLines(1);
        pdown_Selector_1.setLabel("Main Plot");
        pdown_Selector_1.setVertical();
        Selector_1.setBounds (pdown_Selector_1, 0,0,204,108);
        pdown_Selector_1.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(pdown_Selector_1);
        Vector Selector_1_vn = new Vector(3);
        Vector Selector_1_vo = new Vector(3);
        Selector_1_vn.addElement("food group");
        Selector_1_vo.addElement("0");
        Selector_1_vn.addElement("item");
        Selector_1_vo.addElement("1");
        Selector_1_vn.addElement("unit");
        Selector_1_vo.addElement("3");
        Selector_1.setValues(Selector_1_vn, Selector_1_vo);
        Selector_1.selectOption(1);


        PickNode Pick_1 = new PickNode (this.network, "Pick", 1, "Pick_1");
        this.network.addElement((Object)Pick_1);


        WebOptionsNode WebOptions_10 = new WebOptionsNode (this.network, "WebOptions", 10, "WebOptions");
        this.network.addElement((Object)WebOptions_10);


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[1.92754 1.8494 0]");
        Image_2.setInputValueString(6, "[1.92754 1.8494 27.9747]");
        Image_2.setInputValueString(7, "14.9916");
        Image_2.setInputValueString(10, "[0 1 0]");
        Image_2.setInputValueString(41, "pick");
        Image_2.setInputValueString(42, "Pickable Plot");
        Image_2.addInputArc (49, WebOptions_10, 1);


        WebOptionsNode WebOptions_11 = new WebOptionsNode (this.network, "WebOptions", 11, "WebOptions");
        this.network.addElement((Object)WebOptions_11);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(5, "[6.92754 4.52749 0]");
        Image_3.setInputValueString(6, "[6.92754 4.52749 56.3749]");
        Image_3.setInputValueString(7, "30.2113");
        Image_3.setInputValueString(10, "[0 1 0]");
        Image_3.setInputValueString(41, "none");
        Image_3.addInputArc (49, WebOptions_11, 1);


        WebOptionsNode WebOptions_12 = new WebOptionsNode (this.network, "WebOptions", 12, "WebOptions");
        this.network.addElement((Object)WebOptions_12);


        ImageNode Image_4 = new ImageNode (this.network, "Image", 4, "Image");
        this.network.addElement((Object)Image_4);
        Image_4.setInputValueString(5, "[3.07962 8.15925 2.74122]");
        Image_4.setInputValueString(6, "[36.347 -19.6507 26.44]");
        Image_4.setInputValueString(7, "26.481");
        Image_4.setInputValueString(10, "[-0.387822 0.283476 0.877061]");
        Image_4.setInputValueString(41, "none");
        Image_4.addInputArc (49, WebOptions_12, 1);
    }
}
