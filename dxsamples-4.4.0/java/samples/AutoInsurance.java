//
// Applet file generated automatically 
// time: Thu Dec  1 12:14:04 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class AutoInsurance extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_1 = new ControlPanel(this.network, "K Means", 1);
        this.network.addPanel(cp_1);

        ControlPanel cp_2 = new ControlPanel(this.network, "Height", 2);
        this.network.addPanel(cp_2);

        ControlPanel cp_3 = new ControlPanel(this.network, "Colors", 3);
        this.network.addPanel(cp_3);



        ToggleNode Toggle_2 = new ToggleNode (this.network, "Toggle", 2, "Toggle");
        this.network.addElement((Object)Toggle_2);

        Toggle toggle_Toggle_2 = new Toggle();
        Toggle_2.addInteractor(toggle_Toggle_2);
        toggle_Toggle_2.setStyle(1);
        toggle_Toggle_2.setLabelLines(1);
        toggle_Toggle_2.setLabel("Do K Means clustering");
        toggle_Toggle_2.setHorizontal();
        Toggle_2.setBounds (toggle_Toggle_2, 73,140,215,32);
        toggle_Toggle_2.setOutputType(BinaryInstance.INTEGER);
        cp_1.addInteractor(toggle_Toggle_2);
        Vector Toggle_2_vn = new Vector(2);
        Vector Toggle_2_vo = new Vector(2);
        Toggle_2_vn.addElement("set_value");
        Toggle_2_vo.addElement("2");
        Toggle_2_vn.addElement("set_value");
        Toggle_2_vo.addElement("1");
        Toggle_2.setValues(Toggle_2_vn, Toggle_2_vo);


        ScalarNode Scalar_1 = new ScalarNode (this.network, "Scalar", 1, "Scalar");
        this.network.addElement((Object)Scalar_1);

        StepperInteractor step_Scalar_1 = new StepperInteractor(1);
        Scalar_1.addInteractor(step_Scalar_1);
        step_Scalar_1.setStyle(1);
        step_Scalar_1.setLabelLines(1);
        step_Scalar_1.setLabel("Grid Size (miles)");
        step_Scalar_1.setVertical();
        Scalar_1.setBounds (step_Scalar_1, 194,34,165,52);
        cp_1.addInteractor(step_Scalar_1);
        Scalar_1.setValues(1,0,1000,1,1,50);


        ScalarNode Vector_1 = new ScalarNode (this.network, "Vector", 1, "Vector");
        this.network.addElement((Object)Vector_1);

        StepperInteractor step_Vector_1 = new StepperInteractor(2);
        Vector_1.addInteractor(step_Vector_1);
        step_Vector_1.setVector();
        step_Vector_1.setStyle(1);
        step_Vector_1.setLabelLines(1);
        step_Vector_1.setLabel("Grid Offset");
        step_Vector_1.setVertical();
        Vector_1.setBounds (step_Vector_1, 2,22,165,77);
        cp_1.addInteractor(step_Vector_1);
        Vector_1.setValues(1,-100,100,1,1,0);
        Vector_1.setValues(2,-100,100,1,1,0);


        ScalarNode Integer_1 = new ScalarNode (this.network, "Integer", 1, "Integer");
        this.network.addElement((Object)Integer_1);

        StepperInteractor step_Integer_1 = new StepperInteractor(1);
        Integer_1.addInteractor(step_Integer_1);
        step_Integer_1.setStyle(1);
        step_Integer_1.setLabelLines(1);
        step_Integer_1.setLabel("N Clusters");
        step_Integer_1.setVertical();
        Integer_1.setBounds (step_Integer_1, 2,178,165,52);
        cp_1.addInteractor(step_Integer_1);
        Integer_1.setValues(1,-1000000,1000000,1,10);


        ScalarNode Integer_2 = new ScalarNode (this.network, "Integer", 2, "Integer");
        this.network.addElement((Object)Integer_2);

        StepperInteractor step_Integer_2 = new StepperInteractor(1);
        Integer_2.addInteractor(step_Integer_2);
        step_Integer_2.setStyle(1);
        step_Integer_2.setLabelLines(1);
        step_Integer_2.setLabel("N Iterations");
        step_Integer_2.setVertical();
        Integer_2.setBounds (step_Integer_2, 194,178,165,52);
        cp_1.addInteractor(step_Integer_2);
        Integer_2.setValues(1,-1000000,1000000,1,10);


        SelectionNode Selector_4 = new SelectionNode (this.network, "Selector", 4, "Selector");
        this.network.addElement((Object)Selector_4);

        RadioGroup pdown_Selector_4 = new RadioGroup();
        Selector_4.addInteractor(pdown_Selector_4);
        pdown_Selector_4.setStyle(1);
        pdown_Selector_4.setLabelLines(1);
        pdown_Selector_4.setLabel("Color statistics");
        pdown_Selector_4.setVertical();
        Selector_4.setBounds (pdown_Selector_4, 187,34,116,133);
        pdown_Selector_4.setOutputType(BinaryInstance.INTEGER);
        cp_3.addInteractor(pdown_Selector_4);
        Vector Selector_4_vn = new Vector(4);
        Vector Selector_4_vo = new Vector(4);
        Selector_4_vn.addElement("count");
        Selector_4_vo.addElement("0");
        Selector_4_vn.addElement("mean");
        Selector_4_vo.addElement("1");
        Selector_4_vn.addElement("sd");
        Selector_4_vo.addElement("2");
        Selector_4_vn.addElement("max");
        Selector_4_vo.addElement("3");
        Selector_4.setValues(Selector_4_vn, Selector_4_vo);
        Selector_4.selectOption(2);


        SelectionNode Selector_3 = new SelectionNode (this.network, "Selector", 3, "Selector");
        this.network.addElement((Object)Selector_3);

        RadioGroup pdown_Selector_3 = new RadioGroup();
        Selector_3.addInteractor(pdown_Selector_3);
        pdown_Selector_3.setStyle(1);
        pdown_Selector_3.setLabelLines(1);
        pdown_Selector_3.setLabel("Color");
        pdown_Selector_3.setVertical();
        Selector_3.setBounds (pdown_Selector_3, 21,34,146,108);
        pdown_Selector_3.setOutputType(BinaryInstance.INTEGER);
        cp_3.addInteractor(pdown_Selector_3);
        Vector Selector_3_vn = new Vector(3);
        Vector Selector_3_vo = new Vector(3);
        Selector_3_vn.addElement("VehicleAge");
        Selector_3_vo.addElement("0");
        Selector_3_vn.addElement("Claim");
        Selector_3_vo.addElement("1");
        Selector_3_vn.addElement("AnnualMileage");
        Selector_3_vo.addElement("2");
        Selector_3.setValues(Selector_3_vn, Selector_3_vo);
        Selector_3.selectOption(1);


        ToggleNode Toggle_1 = new ToggleNode (this.network, "Toggle", 1, "Toggle");
        this.network.addElement((Object)Toggle_1);

        Toggle toggle_Toggle_1 = new Toggle();
        Toggle_1.addInteractor(toggle_Toggle_1);
        toggle_Toggle_1.setStyle(1);
        toggle_Toggle_1.setLabelLines(1);
        toggle_Toggle_1.setLabel("Log height");
        toggle_Toggle_1.setHorizontal();
        Toggle_1.setBounds (toggle_Toggle_1, 14,158,142,32);
        toggle_Toggle_1.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(toggle_Toggle_1);
        Vector Toggle_1_vn = new Vector(2);
        Vector Toggle_1_vo = new Vector(2);
        Toggle_1_vn.addElement("set_value");
        Toggle_1_vo.addElement("2");
        Toggle_1_vn.addElement("set_value");
        Toggle_1_vo.addElement("1");
        Toggle_1.setValues(Toggle_1_vn, Toggle_1_vo);


        SelectionNode Selector_2 = new SelectionNode (this.network, "Selector", 2, "Selector");
        this.network.addElement((Object)Selector_2);

        RadioGroup pdown_Selector_2 = new RadioGroup();
        Selector_2.addInteractor(pdown_Selector_2);
        pdown_Selector_2.setStyle(1);
        pdown_Selector_2.setLabelLines(1);
        pdown_Selector_2.setLabel("Height statistics");
        pdown_Selector_2.setVertical();
        Selector_2.setBounds (pdown_Selector_2, 189,11,122,133);
        pdown_Selector_2.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(pdown_Selector_2);
        Vector Selector_2_vn = new Vector(4);
        Vector Selector_2_vo = new Vector(4);
        Selector_2_vn.addElement("count");
        Selector_2_vo.addElement("0");
        Selector_2_vn.addElement("mean");
        Selector_2_vo.addElement("1");
        Selector_2_vn.addElement("sd");
        Selector_2_vo.addElement("2");
        Selector_2_vn.addElement("max");
        Selector_2_vo.addElement("3");
        Selector_2.setValues(Selector_2_vn, Selector_2_vo);
        Selector_2.selectOption(1);


        SelectionNode Selector_1 = new SelectionNode (this.network, "Selector", 1, "Selector");
        this.network.addElement((Object)Selector_1);

        RadioGroup pdown_Selector_1 = new RadioGroup();
        Selector_1.addInteractor(pdown_Selector_1);
        pdown_Selector_1.setStyle(1);
        pdown_Selector_1.setLabelLines(1);
        pdown_Selector_1.setLabel("Height");
        pdown_Selector_1.setVertical();
        Selector_1.setBounds (pdown_Selector_1, 14,11,146,108);
        pdown_Selector_1.setOutputType(BinaryInstance.INTEGER);
        cp_2.addInteractor(pdown_Selector_1);
        Vector Selector_1_vn = new Vector(3);
        Vector Selector_1_vo = new Vector(3);
        Selector_1_vn.addElement("VehicleAge");
        Selector_1_vo.addElement("0");
        Selector_1_vn.addElement("Claim");
        Selector_1_vo.addElement("1");
        Selector_1_vn.addElement("AnnualMileage");
        Selector_1_vo.addElement("2");
        Selector_1.setValues(Selector_1_vn, Selector_1_vo);
        Selector_1.selectOption(1);


        PickNode Pick_1 = new PickNode (this.network, "Pick", 1, "Pick_1");
        this.network.addElement((Object)Pick_1);


        DXLOutputNode DXLOutput_1 = new DXLOutputNode (this.network, "DXLOutput", 1, "DXLOutput_1");
        this.network.addElement((Object)DXLOutput_1);


        WebOptionsNode WebOptions_4 = new WebOptionsNode (this.network, "WebOptions", 4, "WebOptions");
        this.network.addElement((Object)WebOptions_4);
        WebOptions_4.setInputValueString(1, "gif");


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[1.48884 -0.909953 -0.0369129]");
        Image_1.setInputValueString(6, "[-14.3235 -164.717 101.088]");
        Image_1.setInputValueString(7, "38.6043");
        Image_1.setInputValueString(10, "[0.0453383 0.52159 0.851991]");
        Image_1.setInputValueString(41, "none");
        Image_1.setInputValueString(42, "Claims");
        Image_1.addInputArc (49, WebOptions_4, 1);


        WebOptionsNode WebOptions_5 = new WebOptionsNode (this.network, "WebOptions", 5, "WebOptions");
        this.network.addElement((Object)WebOptions_5);
        WebOptions_5.setInputValueString(1, "gif");


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.setInputValueString(5, "[2.49361 -0.110775 1.41474]");
        Image_2.setInputValueString(6, "[-13.3187 -163.918 102.54]");
        Image_2.setInputValueString(7, "38.5333");
        Image_2.setInputValueString(10, "[0.0453383 0.52159 0.851991]");
        Image_2.setInputValueString(41, "none");
        Image_2.addInputArc (49, WebOptions_5, 1);
    }
}
