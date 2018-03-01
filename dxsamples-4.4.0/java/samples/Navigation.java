//
// Applet file generated automatically 
// time: Fri Dec  2 10:54:18 2005
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
public class Navigation extends WRLApplication  {

    public void readNetwork() {
        ControlPanel cp_5 = new ControlPanel(this.network, "VRML", 5);
        this.network.addPanel(cp_5);


        LabelGroup cp_5_label_0 = new LabelGroup(3);
        cp_5_label_0.setText("If you select vrml, then the next execution will");
        cp_5_label_0.setText("produce a vrml world containing low altitude");
        cp_5_label_0.setText("airways.... perfect for practicing some navigation.");
        cp_5_label_0.setAlignment(Label.LEFT);
        cp_5_label_0.init();
        cp_5_label_0.setBounds (0,71,311,54);
        cp_5.addDecorator(cp_5_label_0);
        ControlPanel cp_6 = new ControlPanel(this.network, "Intersections", 6);
        this.network.addPanel(cp_6);


        LabelGroup cp_6_label_0 = new LabelGroup(2);
        cp_6_label_0.setText("Select this option to see intersection");
        cp_6_label_0.setText("names added to the low altitude airways.");
        cp_6_label_0.setAlignment(Label.LEFT);
        cp_6_label_0.setFont(new Font("Helvetica", Font.BOLD, 10));
        cp_6_label_0.init();
        cp_6_label_0.setBounds (38,0,237,36);
        cp_6.addDecorator(cp_6_label_0);

        LabelGroup cp_6_label_1 = new LabelGroup(2);
        cp_6_label_1.setText("Select this option to see intersection");
        cp_6_label_1.setText("names added to the high altitude airways.");
        cp_6_label_1.setAlignment(Label.LEFT);
        cp_6_label_1.setFont(new Font("Helvetica", Font.BOLD, 10));
        cp_6_label_1.init();
        cp_6_label_1.setBounds (38,117,237,36);
        cp_6.addDecorator(cp_6_label_1);


        ToggleNode Toggle_13 = new ToggleNode (this.network, "Toggle", 13, "Toggle");
        this.network.addElement((Object)Toggle_13);


        ToggleNode Toggle_11 = new ToggleNode (this.network, "Toggle", 11, "Toggle");
        this.network.addElement((Object)Toggle_11);


        ToggleNode Toggle_5 = new ToggleNode (this.network, "Toggle", 5, "Toggle");
        this.network.addElement((Object)Toggle_5);


        ToggleNode Toggle_7 = new ToggleNode (this.network, "Toggle", 7, "Toggle");
        this.network.addElement((Object)Toggle_7);


        ToggleNode Toggle_17 = new ToggleNode (this.network, "Toggle", 17, "Toggle");
        this.network.addElement((Object)Toggle_17);

        Toggle toggle_Toggle_17 = new Toggle();
        Toggle_17.addInteractor(toggle_Toggle_17);
        toggle_Toggle_17.setStyle(1);
        toggle_Toggle_17.setLabelLines(1);
        toggle_Toggle_17.setLabel("Intersection Names");
        toggle_Toggle_17.setHorizontal();
        Toggle_17.setBounds (toggle_Toggle_17, 54,159,181,32);
        toggle_Toggle_17.setOutputType(BinaryInstance.INTEGER);
        cp_6.addInteractor(toggle_Toggle_17);
        Vector Toggle_17_vn = new Vector(2);
        Vector Toggle_17_vo = new Vector(2);
        Toggle_17_vn.addElement("set_value");
        Toggle_17_vo.addElement("1");
        Toggle_17_vn.addElement("set_value");
        Toggle_17_vo.addElement("2");
        Toggle_17.setValues(Toggle_17_vn, Toggle_17_vo);


        ToggleNode Toggle_4 = new ToggleNode (this.network, "Toggle", 4, "Toggle");
        this.network.addElement((Object)Toggle_4);

        Toggle toggle_Toggle_4 = new Toggle();
        Toggle_4.addInteractor(toggle_Toggle_4);
        toggle_Toggle_4.setStyle(1);
        toggle_Toggle_4.setLabelLines(1);
        toggle_Toggle_4.setLabel("Intersection Names");
        toggle_Toggle_4.setHorizontal();
        Toggle_4.setBounds (toggle_Toggle_4, 54,39,181,32);
        toggle_Toggle_4.setOutputType(BinaryInstance.INTEGER);
        cp_6.addInteractor(toggle_Toggle_4);
        Vector Toggle_4_vn = new Vector(2);
        Vector Toggle_4_vo = new Vector(2);
        Toggle_4_vn.addElement("set_value");
        Toggle_4_vo.addElement("1");
        Toggle_4_vn.addElement("set_value");
        Toggle_4_vo.addElement("2");
        Toggle_4.setValues(Toggle_4_vn, Toggle_4_vo);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[19.8231 7.69742 0.45375]");
        Image_1.setInputValueString(6, "[19.8231 7.69742 37.6188]");
        Image_1.setInputValueString(7, "2.00616");
        Image_1.setInputValueString(10, "[0 1 0]");
        Image_1.setInputValueString(41, "panzoom");
        Image_1.setInputValueString(42, "Victor Highways");
        Image_1.addInputArc (49, WebOptions_2, 1);


        WebOptionsNode WebOptions_3 = new WebOptionsNode (this.network, "WebOptions", 3, "WebOptions");
        this.network.addElement((Object)WebOptions_3);


        ImageNode Image_6 = new ImageNode (this.network, "Image", 6, "Image");
        this.network.addElement((Object)Image_6);
        Image_6.setInputValueString(5, "[19.8231 7.69742 0.45375]");
        Image_6.setInputValueString(6, "[19.8231 7.69742 37.6188]");
        Image_6.setInputValueString(7, "2.00616");
        Image_6.setInputValueString(10, "[0 1 0]");
        Image_6.setInputValueString(41, "none");
        Image_6.setInputValueString(42, "Jet Airways");
        Image_6.addInputArc (49, WebOptions_3, 1);


        WebOptionsNode WebOptions_4 = new WebOptionsNode (this.network, "WebOptions", 4, "WebOptions");
        this.network.addElement((Object)WebOptions_4);


        ImageNode Image_7 = new ImageNode (this.network, "Image", 7, "Image");
        this.network.addElement((Object)Image_7);
        Image_7.setInputValueString(5, "[19.8231 7.69742 0.45375]");
        Image_7.setInputValueString(6, "[19.8231 7.69742 37.6188]");
        Image_7.setInputValueString(7, "2.00616");
        Image_7.setInputValueString(10, "[0 1 0]");
        Image_7.setInputValueString(41, "none");
        Image_7.setInputValueString(42, "Airports");
        Image_7.addInputArc (49, WebOptions_4, 1);


        ToggleNode Toggle_18 = new ToggleNode (this.network, "Toggle", 18, "Toggle");
        this.network.addElement((Object)Toggle_18);

        Toggle toggle_Toggle_18 = new Toggle();
        Toggle_18.addInteractor(toggle_Toggle_18);
        toggle_Toggle_18.setStyle(1);
        toggle_Toggle_18.setLabelLines(1);
        toggle_Toggle_18.setLabel("VRML");
        toggle_Toggle_18.setHorizontal();
        Toggle_18.setBounds (toggle_Toggle_18, 0,26,172,32);
        toggle_Toggle_18.setOutputType(BinaryInstance.INTEGER);
        cp_5.addInteractor(toggle_Toggle_18);
        Vector Toggle_18_vn = new Vector(2);
        Vector Toggle_18_vo = new Vector(2);
        Toggle_18_vn.addElement("set_value");
        Toggle_18_vo.addElement("1");
        Toggle_18_vn.addElement("set_value");
        Toggle_18_vo.addElement("0");
        Toggle_18.setValues(Toggle_18_vn, Toggle_18_vo);


        WebOptionsNode WebOptions_5 = new WebOptionsNode (this.network, "WebOptions", 5, "WebOptions");
        this.network.addElement((Object)WebOptions_5);
        WebOptions_5.setInputValueString(1, "wrl");


        ImageNode Image_8 = new ImageNode (this.network, "Image", 8, "Image");
        this.network.addElement((Object)Image_8);
        Image_8.setInputValueString(5, "[19.3012 5.9631 0.45375]");
        Image_8.setInputValueString(6, "[19.3012 5.9631 191.346]");
        Image_8.setInputValueString(7, "4.43295");
        Image_8.setInputValueString(10, "[0 1 0]");
        Image_8.setInputValueString(41, "none");
        Image_8.addInputArc (49, WebOptions_5, 1);
    }
}
