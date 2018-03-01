//
// Applet file generated automatically 
// time: Fri Dec  2 11:49:53 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class ScatterData extends DXLinkApplication {

    public void readNetwork() {
        ControlPanel cp_3 = new ControlPanel(this.network, "Data Selection", 3);
        this.network.addPanel(cp_3);

        ControlPanel cp_4 = new ControlPanel(this.network, "Import", 4);
        this.network.addPanel(cp_4);

        ControlPanel cp_5 = new ControlPanel(this.network, "Glyph Control", 5);
        this.network.addPanel(cp_5);



        ValueNode StringList_2 = new ValueNode (this.network, "StringList", 2, "StringList");
        this.network.addElement((Object)StringList_2);
        Interactor tfld_StringList_2 = new Interactor();
        StringList_2.addInteractor(tfld_StringList_2);
        tfld_StringList_2.setUseQuotes(false);
        tfld_StringList_2.setStyle(1);
        tfld_StringList_2.setLabelLines(1);
        tfld_StringList_2.setLabel("Transmitter input:");
        tfld_StringList_2.setVertical();
        StringList_2.setBounds (tfld_StringList_2, 0,0,327,65);
        cp_4.addInteractor(tfld_StringList_2);
        StringList_2.setValue("{ \"inside_urban_area\", \"outside_urban_area\", \"rural_farm\", \"rural_nonfarm\", \"males\", \"females\", \"age_under_1\", \"age_21\", \"age_65_to_69\", \"in_household_1\", \"in_household_2\", \"in_household_3\", \"in_household_4\", \"in_household_5\", \"water_company\", \"water_drilled_well\", \"water_dug_well\", \"heat_utility_gas\", \"heat_bottled_gas\", \"heat_oil\", \"heat_coal\", \"heat_solar\" }");


        SelectionNode SelectorList_1 = new SelectionNode (this.network, "SelectorList", 1, "SelectorList");
        this.network.addElement((Object)SelectorList_1);

        ListSelector slist_SelectorList_1 = new ListSelector();
        SelectorList_1.addInteractor(slist_SelectorList_1);
        slist_SelectorList_1.setStyle(1);
        slist_SelectorList_1.setMode(true);
        slist_SelectorList_1.setLabelLines(1);
        slist_SelectorList_1.setLabel("Data List: Choose 3 for 3 Dimensions");
        slist_SelectorList_1.setVertical();
        SelectorList_1.setBounds (slist_SelectorList_1, 0,0,272,299);
        slist_SelectorList_1.setOutputType(BinaryInstance.INTEGER|BinaryInstance.LIST);
        cp_3.addInteractor(slist_SelectorList_1);
        Vector SelectorList_1_vn = new Vector(22);
        Vector SelectorList_1_vo = new Vector(22);
        SelectorList_1_vn.addElement("inside_urban_area");
        SelectorList_1_vo.addElement("0");
        SelectorList_1_vn.addElement("outside_urban_area");
        SelectorList_1_vo.addElement("1");
        SelectorList_1_vn.addElement("rural_farm");
        SelectorList_1_vo.addElement("2");
        SelectorList_1_vn.addElement("rural_nonfarm");
        SelectorList_1_vo.addElement("3");
        SelectorList_1_vn.addElement("males");
        SelectorList_1_vo.addElement("4");
        SelectorList_1_vn.addElement("females");
        SelectorList_1_vo.addElement("5");
        SelectorList_1_vn.addElement("age_under_1");
        SelectorList_1_vo.addElement("6");
        SelectorList_1_vn.addElement("age_21");
        SelectorList_1_vo.addElement("7");
        SelectorList_1_vn.addElement("age_65_to_69");
        SelectorList_1_vo.addElement("8");
        SelectorList_1_vn.addElement("in_household_1");
        SelectorList_1_vo.addElement("9");
        SelectorList_1_vn.addElement("in_household_2");
        SelectorList_1_vo.addElement("10");
        SelectorList_1_vn.addElement("in_household_3");
        SelectorList_1_vo.addElement("11");
        SelectorList_1_vn.addElement("in_household_4");
        SelectorList_1_vo.addElement("12");
        SelectorList_1_vn.addElement("in_household_5");
        SelectorList_1_vo.addElement("13");
        SelectorList_1_vn.addElement("water_company");
        SelectorList_1_vo.addElement("14");
        SelectorList_1_vn.addElement("water_drilled_well");
        SelectorList_1_vo.addElement("15");
        SelectorList_1_vn.addElement("water_dug_well");
        SelectorList_1_vo.addElement("16");
        SelectorList_1_vn.addElement("heat_utility_gas");
        SelectorList_1_vo.addElement("17");
        SelectorList_1_vn.addElement("heat_bottled_gas");
        SelectorList_1_vo.addElement("18");
        SelectorList_1_vn.addElement("heat_oil");
        SelectorList_1_vo.addElement("19");
        SelectorList_1_vn.addElement("heat_coal");
        SelectorList_1_vo.addElement("20");
        SelectorList_1_vn.addElement("heat_solar");
        SelectorList_1_vo.addElement("21");
        SelectorList_1.setValues(SelectorList_1_vn, SelectorList_1_vo);
        SelectorList_1.selectOption(5);
        SelectorList_1.selectOption(10);
        SelectorList_1.selectOption(21);


        SelectionNode Selector_4 = new SelectionNode (this.network, "Selector", 4, "Selector");
        this.network.addElement((Object)Selector_4);

        Selector pdown_Selector_4 = new Selector();
        Selector_4.addInteractor(pdown_Selector_4);
        pdown_Selector_4.setStyle(1);
        pdown_Selector_4.setLabelLines(1);
        pdown_Selector_4.setLabel("Data List: Choose 1 for Glyphs");
        pdown_Selector_4.setVertical();
        Selector_4.setBounds (pdown_Selector_4, 0,14,273,54);
        pdown_Selector_4.setOutputType(BinaryInstance.INTEGER);
        cp_5.addInteractor(pdown_Selector_4);
        Vector Selector_4_vn = new Vector(22);
        Vector Selector_4_vo = new Vector(22);
        Selector_4_vn.addElement("inside_urban_area");
        Selector_4_vo.addElement("0");
        Selector_4_vn.addElement("outside_urban_area");
        Selector_4_vo.addElement("1");
        Selector_4_vn.addElement("rural_farm");
        Selector_4_vo.addElement("2");
        Selector_4_vn.addElement("rural_nonfarm");
        Selector_4_vo.addElement("3");
        Selector_4_vn.addElement("males");
        Selector_4_vo.addElement("4");
        Selector_4_vn.addElement("females");
        Selector_4_vo.addElement("5");
        Selector_4_vn.addElement("age_under_1");
        Selector_4_vo.addElement("6");
        Selector_4_vn.addElement("age_21");
        Selector_4_vo.addElement("7");
        Selector_4_vn.addElement("age_65_to_69");
        Selector_4_vo.addElement("8");
        Selector_4_vn.addElement("in_household_1");
        Selector_4_vo.addElement("9");
        Selector_4_vn.addElement("in_household_2");
        Selector_4_vo.addElement("10");
        Selector_4_vn.addElement("in_household_3");
        Selector_4_vo.addElement("11");
        Selector_4_vn.addElement("in_household_4");
        Selector_4_vo.addElement("12");
        Selector_4_vn.addElement("in_household_5");
        Selector_4_vo.addElement("13");
        Selector_4_vn.addElement("water_company");
        Selector_4_vo.addElement("14");
        Selector_4_vn.addElement("water_drilled_well");
        Selector_4_vo.addElement("15");
        Selector_4_vn.addElement("water_dug_well");
        Selector_4_vo.addElement("16");
        Selector_4_vn.addElement("heat_utility_gas");
        Selector_4_vo.addElement("17");
        Selector_4_vn.addElement("heat_bottled_gas");
        Selector_4_vo.addElement("18");
        Selector_4_vn.addElement("heat_oil");
        Selector_4_vo.addElement("19");
        Selector_4_vn.addElement("heat_coal");
        Selector_4_vo.addElement("20");
        Selector_4_vn.addElement("heat_solar");
        Selector_4_vo.addElement("21");
        Selector_4.setValues(Selector_4_vn, Selector_4_vo);
        Selector_4.selectOption(22);


        ScalarNode Scalar_2 = new ScalarNode (this.network, "Scalar", 2, "Scalar");
        this.network.addElement((Object)Scalar_2);

        StepperInteractor step_Scalar_2 = new StepperInteractor(1);
        Scalar_2.addInteractor(step_Scalar_2);
        step_Scalar_2.setStyle(1);
        step_Scalar_2.setLabelLines(1);
        step_Scalar_2.setLabel("scaling for glyph size");
        step_Scalar_2.setVertical();
        Scalar_2.setBounds (step_Scalar_2, 55,91,158,52);
        cp_5.addInteractor(step_Scalar_2);
        Scalar_2.setValues(1,0.1,20,0.1,5,2.6);


        WebOptionsNode WebOptions_4 = new WebOptionsNode (this.network, "WebOptions", 4, "WebOptions");
        this.network.addElement((Object)WebOptions_4);


        ImageNode Image_3 = new ImageNode (this.network, "Image", 3, "Image");
        this.network.addElement((Object)Image_3);
        Image_3.setInputValueString(5, "[15.9363 2.79351 0.500823]");
        Image_3.setInputValueString(6, "[19.1203 5.97752 4.42259]");
        Image_3.setInputValueString(7, "3.2");
        Image_3.setInputValueString(10, "[0 1 0]");
        Image_3.setInputValueString(41, "none");
        Image_3.setInputValueString(42, "Scattered Glyphs");
        Image_3.addInputArc (49, WebOptions_4, 1);
    }
}
