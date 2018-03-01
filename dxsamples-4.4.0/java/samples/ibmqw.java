//
// Applet file generated automatically 
// time: Fri Dec  2 15:50:55 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class ibmqw extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Controls", 1);
        this.network.addPanel(cp_1);



        SelectionNode Selector_1 = new SelectionNode (this.network, "Selector", 1, "Selector");
        this.network.addElement((Object)Selector_1);

        RadioGroup pdown_Selector_1 = new RadioGroup();
        Selector_1.addInteractor(pdown_Selector_1);
        pdown_Selector_1.setStyle(0);
        pdown_Selector_1.setLabelLines(1);
        pdown_Selector_1.setLabel("Color by");
        pdown_Selector_1.setVertical();
        Selector_1.setBounds (pdown_Selector_1, 0,11,161,158);
        cp_1.addInteractor(pdown_Selector_1);
        Vector Selector_1_vn = new Vector(5);
        Vector Selector_1_vo = new Vector(5);
        Selector_1_vn.addElement("Shares Outstanding");
        Selector_1_vo.addElement("Shares");
        Selector_1_vn.addElement("Total Debt");
        Selector_1_vo.addElement("Debt");
        Selector_1_vn.addElement("Asset Book Value");
        Selector_1_vo.addElement("BookAssets");
        Selector_1_vn.addElement("Earnings");
        Selector_1_vo.addElement("Earnings");
        Selector_1_vn.addElement("Total Inventory");
        Selector_1_vo.addElement("Inventory");
        Selector_1.setValues(Selector_1_vn, Selector_1_vo);
        Selector_1.selectOption(4);


        PickNode Pick_1 = new PickNode (this.network, "Pick", 1, "Pick_1");
        this.network.addElement((Object)Pick_1);


        DXLOutputNode DXLOutput_1 = new DXLOutputNode (this.network, "DXLOutput", 1, "DXLOutput_1");
        this.network.addElement((Object)DXLOutput_1);


        DXLOutputNode DXLOutput_2 = new DXLOutputNode (this.network, "DXLOutput", 2, "DXLOutput_2");
        this.network.addElement((Object)DXLOutput_2);


        ValueNode String_1 = new ValueNode (this.network, "String", 1, "String");
        this.network.addElement((Object)String_1);


        ScalarNode Scalar_3 = new ScalarNode (this.network, "Scalar", 3, "Scalar");
        this.network.addElement((Object)Scalar_3);


        WebOptionsNode WebOptions_1 = new WebOptionsNode (this.network, "WebOptions", 1, "WebOptions");
        this.network.addElement((Object)WebOptions_1);


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[2.5 1995 0.75035]");
        Image_1.setInputValueString(6, "[6.60187 1984.27 5.36357]");
        Image_1.setInputValueString(10, "[-0.107489 0.35769 0.927634]");
        Image_1.setInputValueString(11, "30.0");
        Image_1.setInputValueString(41, "none");
        Image_1.setInputValueString(42, "Stock Price");
        Image_1.addInputArc (49, WebOptions_1, 1);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.addInputArc (49, WebOptions_2, 1);
    }
}
