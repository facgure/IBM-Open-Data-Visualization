//
// Applet file generated automatically 
// time: Fri Dec  2 10:36:36 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class MutualFund extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_6 = new ControlPanel(this.network, "Mutual Fund", 6);
        this.network.addPanel(cp_6);



        SelectionNode Selector_16 = new SelectionNode (this.network, "Selector", 16, "Selector");
        this.network.addElement((Object)Selector_16);

        Selector pdown_Selector_16 = new Selector();
        Selector_16.addInteractor(pdown_Selector_16);
        pdown_Selector_16.setStyle(1);
        pdown_Selector_16.setLabelLines(1);
        pdown_Selector_16.setLabel("Colored By:");
        pdown_Selector_16.setHorizontal();
        Selector_16.setBounds (pdown_Selector_16, 22,64,244,38);
        pdown_Selector_16.setOutputType(BinaryInstance.INTEGER);
        cp_6.addInteractor(pdown_Selector_16);
        Vector Selector_16_vn = new Vector(12);
        Vector Selector_16_vo = new Vector(12);
        Selector_16_vn.addElement("1 Yr");
        Selector_16_vo.addElement("1");
        Selector_16_vn.addElement("3 Yr");
        Selector_16_vo.addElement("2");
        Selector_16_vn.addElement("5 Yr");
        Selector_16_vo.addElement("3");
        Selector_16_vn.addElement("10 Yr");
        Selector_16_vo.addElement("4");
        Selector_16_vn.addElement("Since Inception");
        Selector_16_vo.addElement("5");
        Selector_16_vn.addElement("Expense Ratio");
        Selector_16_vo.addElement("7");
        Selector_16_vn.addElement("Load Type");
        Selector_16_vo.addElement("8");
        Selector_16_vn.addElement("NTF Status");
        Selector_16_vo.addElement("9");
        Selector_16_vn.addElement("Net Assets MM");
        Selector_16_vo.addElement("10");
        Selector_16_vn.addElement("Inv. Objs.");
        Selector_16_vo.addElement("12");
        Selector_16_vn.addElement("Datenum");
        Selector_16_vo.addElement("14");
        Selector_16_vn.addElement("sd");
        Selector_16_vo.addElement("15");
        Selector_16.setValues(Selector_16_vn, Selector_16_vo);
        Selector_16.selectOption(6);


        SelectionNode Selector_17 = new SelectionNode (this.network, "Selector", 17, "Selector");
        this.network.addElement((Object)Selector_17);

        Selector pdown_Selector_17 = new Selector();
        Selector_17.addInteractor(pdown_Selector_17);
        pdown_Selector_17.setStyle(1);
        pdown_Selector_17.setLabelLines(1);
        pdown_Selector_17.setLabel("Sized By:");
        pdown_Selector_17.setHorizontal();
        Selector_17.setBounds (pdown_Selector_17, 22,12,244,38);
        pdown_Selector_17.setOutputType(BinaryInstance.INTEGER);
        cp_6.addInteractor(pdown_Selector_17);
        Vector Selector_17_vn = new Vector(16);
        Vector Selector_17_vo = new Vector(16);
        Selector_17_vn.addElement("Name");
        Selector_17_vo.addElement("0");
        Selector_17_vn.addElement("1 Yr");
        Selector_17_vo.addElement("1");
        Selector_17_vn.addElement("3 Yr");
        Selector_17_vo.addElement("2");
        Selector_17_vn.addElement("5 Yr");
        Selector_17_vo.addElement("3");
        Selector_17_vn.addElement("10 Yr");
        Selector_17_vo.addElement("4");
        Selector_17_vn.addElement("Since Inception");
        Selector_17_vo.addElement("5");
        Selector_17_vn.addElement("Inception Date");
        Selector_17_vo.addElement("6");
        Selector_17_vn.addElement("Expense Ratio");
        Selector_17_vo.addElement("7");
        Selector_17_vn.addElement("Load Type");
        Selector_17_vo.addElement("8");
        Selector_17_vn.addElement("NTF Status");
        Selector_17_vo.addElement("9");
        Selector_17_vn.addElement("Net Assets MM");
        Selector_17_vo.addElement("10");
        Selector_17_vn.addElement("Net Assets Date");
        Selector_17_vo.addElement("11");
        Selector_17_vn.addElement("Inv. Objs.");
        Selector_17_vo.addElement("12");
        Selector_17_vn.addElement("Fund Symbol");
        Selector_17_vo.addElement("13");
        Selector_17_vn.addElement("Datenum");
        Selector_17_vo.addElement("14");
        Selector_17_vn.addElement("sd");
        Selector_17_vo.addElement("15");
        Selector_17.setValues(Selector_17_vn, Selector_17_vo);
        Selector_17.selectOption(6);


        PickNode Pick_3 = new PickNode (this.network, "Pick", 3, "Pick_3");
        this.network.addElement((Object)Pick_3);


        SelectionNode Selector_22 = new SelectionNode (this.network, "Selector", 22, "Selector");
        this.network.addElement((Object)Selector_22);

        Selector pdown_Selector_22 = new Selector();
        Selector_22.addInteractor(pdown_Selector_22);
        pdown_Selector_22.setStyle(1);
        pdown_Selector_22.setLabelLines(1);
        pdown_Selector_22.setLabel("Pick Output");
        pdown_Selector_22.setHorizontal();
        Selector_22.setBounds (pdown_Selector_22, 22,116,243,38);
        pdown_Selector_22.setOutputType(BinaryInstance.INTEGER);
        cp_6.addInteractor(pdown_Selector_22);
        Vector Selector_22_vn = new Vector(16);
        Vector Selector_22_vo = new Vector(16);
        Selector_22_vn.addElement("Name");
        Selector_22_vo.addElement("0");
        Selector_22_vn.addElement("1 Yr");
        Selector_22_vo.addElement("1");
        Selector_22_vn.addElement("3 Yr");
        Selector_22_vo.addElement("2");
        Selector_22_vn.addElement("5 Yr");
        Selector_22_vo.addElement("3");
        Selector_22_vn.addElement("10 Yr");
        Selector_22_vo.addElement("4");
        Selector_22_vn.addElement("Since Inception");
        Selector_22_vo.addElement("5");
        Selector_22_vn.addElement("Inception Date");
        Selector_22_vo.addElement("6");
        Selector_22_vn.addElement("Expense Ratio");
        Selector_22_vo.addElement("7");
        Selector_22_vn.addElement("Load Type");
        Selector_22_vo.addElement("8");
        Selector_22_vn.addElement("NTF Status");
        Selector_22_vo.addElement("9");
        Selector_22_vn.addElement("Net Assets MM");
        Selector_22_vo.addElement("10");
        Selector_22_vn.addElement("Net Assets Date");
        Selector_22_vo.addElement("11");
        Selector_22_vn.addElement("Inv. Objs.");
        Selector_22_vo.addElement("12");
        Selector_22_vn.addElement("Fund Symbol");
        Selector_22_vo.addElement("13");
        Selector_22_vn.addElement("Datenum");
        Selector_22_vo.addElement("14");
        Selector_22_vn.addElement("sd");
        Selector_22_vo.addElement("15");
        Selector_22.setValues(Selector_22_vn, Selector_22_vo);
        Selector_22.selectOption(1);


        DXLOutputNode DXLOutput_1 = new DXLOutputNode (this.network, "DXLOutput", 1, "PickStr");
        this.network.addElement((Object)DXLOutput_1);


        DXLOutputNode DXLOutput_2 = new DXLOutputNode (this.network, "DXLOutput", 2, "SizeStr");
        this.network.addElement((Object)DXLOutput_2);


        WebOptionsNode WebOptions_15 = new WebOptionsNode (this.network, "WebOptions", 15, "WebOptions");
        this.network.addElement((Object)WebOptions_15);


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[0.638699 0.528505 0.641394]");
        Image_2.setInputValueString(6, "[1.6871 1.57691 4.55166]");
        Image_2.setInputValueString(10, "[0 1 0]");
        Image_2.setInputValueString(11, "30.0");
        Image_2.setInputValueString(41, "rotate");
        Image_2.setInputValueString(42, "Graph");
        Image_2.addInputArc (49, WebOptions_15, 1);


        WebOptionsNode WebOptions_16 = new WebOptionsNode (this.network, "WebOptions", 16, "WebOptions");
        this.network.addElement((Object)WebOptions_16);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(10, "[0 1 0]");
        Image_3.addInputArc (49, WebOptions_16, 1);
    }
}
