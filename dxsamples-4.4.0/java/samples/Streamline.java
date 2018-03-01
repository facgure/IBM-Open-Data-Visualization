//
// Applet file generated automatically 
// time: Fri Dec  2 14:26:15 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class Streamline extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Options", 1);
        this.network.addPanel(cp_1);



        SelectionNode Selector_2 = new SelectionNode (this.network, "Selector", 2, "Selector");
        this.network.addElement((Object)Selector_2);

        Selector pdown_Selector_2 = new Selector();
        Selector_2.addInteractor(pdown_Selector_2);
        pdown_Selector_2.setStyle(1);
        pdown_Selector_2.setLabelLines(1);
        pdown_Selector_2.setLabel("Colors");
        pdown_Selector_2.setVertical();
        Selector_2.setBounds (pdown_Selector_2, 22,8,195,54);
        pdown_Selector_2.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(pdown_Selector_2);
        Vector Selector_2_vn = new Vector(2);
        Vector Selector_2_vo = new Vector(2);
        Selector_2_vn.addElement("based on time");
        Selector_2_vo.addElement("1");
        Selector_2_vn.addElement("based on velocity");
        Selector_2_vo.addElement("2");
        Selector_2.setValues(Selector_2_vn, Selector_2_vo);
        Selector_2.selectOption(2);


        SelectionNode Selector_3 = new SelectionNode (this.network, "Selector", 3, "Selector");
        this.network.addElement((Object)Selector_3);

        Selector pdown_Selector_3 = new Selector();
        Selector_3.addInteractor(pdown_Selector_3);
        pdown_Selector_3.setStyle(1);
        pdown_Selector_3.setLabelLines(1);
        pdown_Selector_3.setLabel("Sequencer Control");
        pdown_Selector_3.setVertical();
        Selector_3.setBounds (pdown_Selector_3, 12,100,271,54);
        pdown_Selector_3.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(pdown_Selector_3);
        Vector Selector_3_vn = new Vector(2);
        Vector Selector_3_vo = new Vector(2);
        Selector_3_vn.addElement("moves starting points");
        Selector_3_vo.addElement("1");
        Selector_3_vn.addElement("changes head of streamlines");
        Selector_3_vo.addElement("2");
        Selector_3.setValues(Selector_3_vn, Selector_3_vo);
        Selector_3.selectOption(2);


        SequencerNode Sequencer_1 = new SequencerNode (this.network, "Sequencer", 1, "Sequencer");
        this.network.addElement((Object)Sequencer_1);
        Sequencer_1.setInputValueString(4, "1");
        Sequencer_1.setInputValueString(5, "12");
        Sequencer_1.setInputValueString(6, "1");


        WebOptionsNode WebOptions_1 = new WebOptionsNode (this.network, "WebOptions", 1, "WebOptions");
        this.network.addElement((Object)WebOptions_1);


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[5.99952 11.107 8.99996]");
        Image_2.setInputValueString(6, "[31.4441 28.9007 59.118]");
        Image_2.setInputValueString(7, "18.6096");
        Image_2.setInputValueString(10, "[-0.080195 0.951475 -0.297093]");
        Image_2.setInputValueString(41, "none");
        Image_2.setInputValueString(42, "Streamlines");
        Image_2.addInputArc (49, WebOptions_1, 1);


        WebOptionsNode WebOptions_2 = new WebOptionsNode (this.network, "WebOptions", 2, "WebOptions");
        this.network.addElement((Object)WebOptions_2);


        ImageNode Image_7 = new ImageNode (this.network, "Image", 7, "Image");
        this.network.addElement((Object)Image_7);
        Image_7.addInputArc (49, WebOptions_2, 1);
    }
}
