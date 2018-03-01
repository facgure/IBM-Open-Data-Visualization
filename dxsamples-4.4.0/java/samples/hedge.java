//
// Applet file generated automatically 
// time: Fri Dec  2 15:32:04 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class hedge extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "Common", 1);
        this.network.addPanel(cp_1);

        ControlPanel cp_4 = new ControlPanel(this.network, "Security 1", 4);
        this.network.addPanel(cp_4);

        ControlPanel cp_6 = new ControlPanel(this.network, "Security 2", 6);
        this.network.addPanel(cp_6);

        ControlPanel cp_7 = new ControlPanel(this.network, "Security 3", 7);
        this.network.addPanel(cp_7);



        SelectionNode Selector_1 = new SelectionNode (this.network, "Selector", 1, "Selector");
        this.network.addElement((Object)Selector_1);

        Selector pdown_Selector_1 = new Selector();
        Selector_1.addInteractor(pdown_Selector_1);
        pdown_Selector_1.setStyle(0);
        pdown_Selector_1.setLabelLines(0);
        pdown_Selector_1.setVertical();
        Selector_1.setBounds (pdown_Selector_1, 0,0,172,40);
        pdown_Selector_1.setOutputType(BinaryInstance.INTEGER);
        cp_4.addInteractor(pdown_Selector_1);
        Vector Selector_1_vn = new Vector(7);
        Vector Selector_1_vo = new Vector(7);
        Selector_1_vn.addElement("Short");
        Selector_1_vo.addElement("3");
        Selector_1_vn.addElement("Buy Put");
        Selector_1_vo.addElement("2");
        Selector_1_vn.addElement("Buy Call");
        Selector_1_vo.addElement("1");
        Selector_1_vn.addElement("Write Call");
        Selector_1_vo.addElement("4");
        Selector_1_vn.addElement("Write Put");
        Selector_1_vo.addElement("5");
        Selector_1_vn.addElement("Long");
        Selector_1_vo.addElement("6");
        Selector_1_vn.addElement("Nothing");
        Selector_1_vo.addElement("7");
        Selector_1.setValues(Selector_1_vn, Selector_1_vo);
        Selector_1.selectOption(6);


        ScalarNode Scalar_12 = new ScalarNode (this.network, "Scalar", 12, "Scalar");
        this.network.addElement((Object)Scalar_12);

        StepperInteractor step_Scalar_12 = new StepperInteractor(1);
        Scalar_12.addInteractor(step_Scalar_12);
        step_Scalar_12.setStyle(0);
        step_Scalar_12.setLabelLines(1);
        step_Scalar_12.setLabel("Purchase Price");
        step_Scalar_12.setVertical();
        Scalar_12.setBounds (step_Scalar_12, 0,88,240,52);
        cp_1.addInteractor(step_Scalar_12);
        Scalar_12.setValues(1,0,1000,1,2,50);


        ScalarNode Scalar_2 = new ScalarNode (this.network, "Scalar", 2, "Scalar");
        this.network.addElement((Object)Scalar_2);

        StepperInteractor step_Scalar_2 = new StepperInteractor(1);
        Scalar_2.addInteractor(step_Scalar_2);
        step_Scalar_2.setStyle(0);
        step_Scalar_2.setLabelLines(1);
        step_Scalar_2.setLabel("Interest Rate (1+r)");
        step_Scalar_2.setVertical();
        Scalar_2.setBounds (step_Scalar_2, 247,88,240,52);
        cp_1.addInteractor(step_Scalar_2);
        Scalar_2.setValues(1,1,1.5,0.002,3,1.15);


        SequencerNode Sequencer_1 = new SequencerNode (this.network, "Sequencer", 1, "Sequencer");
        this.network.addElement((Object)Sequencer_1);
        Sequencer_1.setInputValueString(4, "1");
        Sequencer_1.setInputValueString(5, "365");
        Sequencer_1.setInputValueString(6, "10");


        ToggleNode Toggle_2 = new ToggleNode (this.network, "Toggle", 2, "Toggle");
        this.network.addElement((Object)Toggle_2);

        Toggle toggle_Toggle_2 = new Toggle();
        Toggle_2.addInteractor(toggle_Toggle_2);
        toggle_Toggle_2.setStyle(0);
        toggle_Toggle_2.setLabelLines(1);
        toggle_Toggle_2.setLabel("Share Values");
        toggle_Toggle_2.setHorizontal();
        Toggle_2.setBounds (toggle_Toggle_2, 0,44,240,32);
        toggle_Toggle_2.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(toggle_Toggle_2);
        Vector Toggle_2_vn = new Vector(2);
        Vector Toggle_2_vo = new Vector(2);
        Toggle_2_vn.addElement("set_value");
        Toggle_2_vo.addElement("1");
        Toggle_2_vn.addElement("set_value");
        Toggle_2_vo.addElement("2");
        Toggle_2.setValues(Toggle_2_vn, Toggle_2_vo);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(0);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Strike Price");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 247,0,240,52);
        cp_1.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,0,1000,1,2,50);


        ScalarNode Scalar_3 = new ScalarNode (this.network, "Scalar", 3, "Scalar");
        this.network.addElement((Object)Scalar_3);

        StepperInteractor step_Scalar_3 = new StepperInteractor(1);
        Scalar_3.addInteractor(step_Scalar_3);
        step_Scalar_3.setStyle(0);
        step_Scalar_3.setLabelLines(1);
        step_Scalar_3.setLabel("Strike Price");
        step_Scalar_3.setVertical();
        Scalar_3.setBounds (step_Scalar_3, 0,66,172,52);
        cp_4.addInteractor(step_Scalar_3);
        Scalar_3.setValues(1,0,1000,1,2,50);


        ScalarNode Scalar_4 = new ScalarNode (this.network, "Scalar", 4, "Scalar");
        this.network.addElement((Object)Scalar_4);

        StepperInteractor step_Scalar_4 = new StepperInteractor(1);
        Scalar_4.addInteractor(step_Scalar_4);
        step_Scalar_4.setStyle(0);
        step_Scalar_4.setLabelLines(1);
        step_Scalar_4.setLabel("Call Price");
        step_Scalar_4.setVertical();
        Scalar_4.setBounds (step_Scalar_4, 200,0,172,52);
        cp_4.addInteractor(step_Scalar_4);
        Scalar_4.setValues(1,0,1000,1,2,5);


        ScalarNode Scalar_9 = new ScalarNode (this.network, "Scalar", 9, "Scalar");
        this.network.addElement((Object)Scalar_9);

        StepperInteractor step_Scalar_9 = new StepperInteractor(1);
        Scalar_9.addInteractor(step_Scalar_9);
        step_Scalar_9.setStyle(0);
        step_Scalar_9.setLabelLines(1);
        step_Scalar_9.setLabel("Purchase Price");
        step_Scalar_9.setVertical();
        Scalar_9.setBounds (step_Scalar_9, 200,66,172,52);
        cp_4.addInteractor(step_Scalar_9);
        Scalar_9.setValues(1,0,1000,1,2,50);


        SelectionNode Selector_5 = new SelectionNode (this.network, "Selector", 5, "Selector");
        this.network.addElement((Object)Selector_5);

        Selector pdown_Selector_5 = new Selector();
        Selector_5.addInteractor(pdown_Selector_5);
        pdown_Selector_5.setStyle(0);
        pdown_Selector_5.setLabelLines(0);
        pdown_Selector_5.setVertical();
        Selector_5.setBounds (pdown_Selector_5, 0,0,169,40);
        pdown_Selector_5.setOutputType(BinaryInstance.INTEGER);
        cp_6.addInteractor(pdown_Selector_5);
        Vector Selector_5_vn = new Vector(7);
        Vector Selector_5_vo = new Vector(7);
        Selector_5_vn.addElement("Short");
        Selector_5_vo.addElement("3");
        Selector_5_vn.addElement("Buy Put");
        Selector_5_vo.addElement("2");
        Selector_5_vn.addElement("Buy Call");
        Selector_5_vo.addElement("1");
        Selector_5_vn.addElement("Write Call");
        Selector_5_vo.addElement("4");
        Selector_5_vn.addElement("Write Put");
        Selector_5_vo.addElement("5");
        Selector_5_vn.addElement("Nothing");
        Selector_5_vo.addElement("7");
        Selector_5_vn.addElement("Long");
        Selector_5_vo.addElement("6");
        Selector_5.setValues(Selector_5_vn, Selector_5_vo);
        Selector_5.selectOption(4);


        ScalarNode Scalar_5 = new ScalarNode (this.network, "Scalar", 5, "Scalar");
        this.network.addElement((Object)Scalar_5);

        StepperInteractor step_Scalar_5 = new StepperInteractor(1);
        Scalar_5.addInteractor(step_Scalar_5);
        step_Scalar_5.setStyle(0);
        step_Scalar_5.setLabelLines(1);
        step_Scalar_5.setLabel("Strike Price");
        step_Scalar_5.setVertical();
        Scalar_5.setBounds (step_Scalar_5, 0,66,169,52);
        cp_6.addInteractor(step_Scalar_5);
        Scalar_5.setValues(1,0,1000,1,2,50);


        ScalarNode Scalar_6 = new ScalarNode (this.network, "Scalar", 6, "Scalar");
        this.network.addElement((Object)Scalar_6);

        StepperInteractor step_Scalar_6 = new StepperInteractor(1);
        Scalar_6.addInteractor(step_Scalar_6);
        step_Scalar_6.setStyle(0);
        step_Scalar_6.setLabelLines(1);
        step_Scalar_6.setLabel("Call Price");
        step_Scalar_6.setVertical();
        Scalar_6.setBounds (step_Scalar_6, 198,0,169,52);
        cp_6.addInteractor(step_Scalar_6);
        Scalar_6.setValues(1,0,1000,1,2,5);


        ScalarNode Scalar_10 = new ScalarNode (this.network, "Scalar", 10, "Scalar");
        this.network.addElement((Object)Scalar_10);

        StepperInteractor step_Scalar_10 = new StepperInteractor(1);
        Scalar_10.addInteractor(step_Scalar_10);
        step_Scalar_10.setStyle(0);
        step_Scalar_10.setLabelLines(1);
        step_Scalar_10.setLabel("Purchase Price");
        step_Scalar_10.setVertical();
        Scalar_10.setBounds (step_Scalar_10, 198,66,169,52);
        cp_6.addInteractor(step_Scalar_10);
        Scalar_10.setValues(1,0,1000,1,2,50);


        SelectionNode Selector_6 = new SelectionNode (this.network, "Selector", 6, "Selector");
        this.network.addElement((Object)Selector_6);

        Selector pdown_Selector_6 = new Selector();
        Selector_6.addInteractor(pdown_Selector_6);
        pdown_Selector_6.setStyle(0);
        pdown_Selector_6.setLabelLines(0);
        pdown_Selector_6.setVertical();
        Selector_6.setBounds (pdown_Selector_6, 0,0,170,40);
        pdown_Selector_6.setOutputType(BinaryInstance.INTEGER);
        cp_7.addInteractor(pdown_Selector_6);
        Vector Selector_6_vn = new Vector(7);
        Vector Selector_6_vo = new Vector(7);
        Selector_6_vn.addElement("Short");
        Selector_6_vo.addElement("3");
        Selector_6_vn.addElement("Buy Put");
        Selector_6_vo.addElement("2");
        Selector_6_vn.addElement("Buy Call");
        Selector_6_vo.addElement("1");
        Selector_6_vn.addElement("Write Call");
        Selector_6_vo.addElement("4");
        Selector_6_vn.addElement("Write Put");
        Selector_6_vo.addElement("5");
        Selector_6_vn.addElement("Long");
        Selector_6_vo.addElement("6");
        Selector_6_vn.addElement("Nothing");
        Selector_6_vo.addElement("7");
        Selector_6.setValues(Selector_6_vn, Selector_6_vo);
        Selector_6.selectOption(4);


        ScalarNode Scalar_7 = new ScalarNode (this.network, "Scalar", 7, "Scalar");
        this.network.addElement((Object)Scalar_7);

        StepperInteractor step_Scalar_7 = new StepperInteractor(1);
        Scalar_7.addInteractor(step_Scalar_7);
        step_Scalar_7.setStyle(0);
        step_Scalar_7.setLabelLines(1);
        step_Scalar_7.setLabel("Strike Price");
        step_Scalar_7.setVertical();
        Scalar_7.setBounds (step_Scalar_7, 0,66,170,52);
        cp_7.addInteractor(step_Scalar_7);
        Scalar_7.setValues(1,0,1000,1,2,50);


        ScalarNode Scalar_8 = new ScalarNode (this.network, "Scalar", 8, "Scalar");
        this.network.addElement((Object)Scalar_8);

        StepperInteractor step_Scalar_8 = new StepperInteractor(1);
        Scalar_8.addInteractor(step_Scalar_8);
        step_Scalar_8.setStyle(0);
        step_Scalar_8.setLabelLines(1);
        step_Scalar_8.setLabel("Call Price");
        step_Scalar_8.setVertical();
        Scalar_8.setBounds (step_Scalar_8, 197,0,170,52);
        cp_7.addInteractor(step_Scalar_8);
        Scalar_8.setValues(1,0,1000,1,2,5);


        ScalarNode Scalar_11 = new ScalarNode (this.network, "Scalar", 11, "Scalar");
        this.network.addElement((Object)Scalar_11);

        StepperInteractor step_Scalar_11 = new StepperInteractor(1);
        Scalar_11.addInteractor(step_Scalar_11);
        step_Scalar_11.setStyle(0);
        step_Scalar_11.setLabelLines(1);
        step_Scalar_11.setLabel("Purchase Price");
        step_Scalar_11.setVertical();
        Scalar_11.setBounds (step_Scalar_11, 197,66,170,52);
        cp_7.addInteractor(step_Scalar_11);
        Scalar_11.setValues(1,0,1000,1,2,50);


        SelectionNode Selector_7 = new SelectionNode (this.network, "Selector", 7, "Selector");
        this.network.addElement((Object)Selector_7);

        Selector pdown_Selector_7 = new Selector();
        Selector_7.addInteractor(pdown_Selector_7);
        pdown_Selector_7.setStyle(0);
        pdown_Selector_7.setLabelLines(0);
        pdown_Selector_7.setVertical();
        Selector_7.setBounds (pdown_Selector_7, 0,0,240,40);
        pdown_Selector_7.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(pdown_Selector_7);
        Vector Selector_7_vn = new Vector(4);
        Vector Selector_7_vo = new Vector(4);
        Selector_7_vn.addElement("Signature Only");
        Selector_7_vo.addElement("2");
        Selector_7_vn.addElement("Securities + Signature");
        Selector_7_vo.addElement("1");
        Selector_7_vn.addElement("Securities Only");
        Selector_7_vo.addElement("3");
        Selector_7_vn.addElement("Signature Colored by N(x)");
        Selector_7_vo.addElement("4");
        Selector_7.setValues(Selector_7_vn, Selector_7_vo);
        Selector_7.selectOption(3);


        DXLOutputNode DXLOutput_1 = new DXLOutputNode (this.network, "DXLOutput", 1, "DXLOutput_1");
        this.network.addElement((Object)DXLOutput_1);


        DXLOutputNode DXLOutput_2 = new DXLOutputNode (this.network, "DXLOutput", 2, "DXLOutput_2");
        this.network.addElement((Object)DXLOutput_2);


        WebOptionsNode WebOptions_1 = new WebOptionsNode (this.network, "WebOptions", 1, "WebOptions");
        this.network.addElement((Object)WebOptions_1);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(5, "[57.5 72.5 2.5]");
        Image_3.setInputValueString(6, "[-66.8957 -293.973 205.956]");
        Image_3.setInputValueString(10, "[0.13856 0.444339 0.885078]");
        Image_3.setInputValueString(11, "30.0");
        Image_3.setInputValueString(41, "rotate");
        Image_3.setInputValueString(42, "Hedge");
        Image_3.addInputArc (49, WebOptions_1, 1);
    }
}
