//
// Applet file generated automatically 
// time: Fri Dec  2 15:14:57 2005
// DX version: 3.2.0 (format), 4.3.3 (DX)
//
// Java version 1.1
//
import dx.net.*;
import dx.runtime.*;
import java.util.*;
import java.awt.*;

public class gordon extends DXLinkApplication {

    public void readNetwork() {


        PickNode Pick_4 = new PickNode (this.network, "Pick", 4, "Pick_4");
        this.network.addElement((Object)Pick_4);


        DrivenNode Colormap_2 = new DrivenNode (this.network, "Colormap", 2, "Colormap");
        this.network.addElement((Object)Colormap_2);


        WebOptionsNode WebOptions_6 = new WebOptionsNode (this.network, "WebOptions", 6, "WebOptions");
        this.network.addElement((Object)WebOptions_6);


        ImageNode Image_1 = new ImageNode (this.network, "Image", 1, "Image");
        this.network.addElement((Object)Image_1);
        Image_1.setInputValueString(5, "[0.050375 0.091 0.052994]");
        Image_1.setInputValueString(6, "[-0.233023 0.401171 0.338656]");
        Image_1.setInputValueString(7, "0.272268");
        Image_1.setInputValueString(10, "[0.41134 -0.385043 0.826161]");
        Image_1.setInputValueString(41, "none");
        Image_1.setInputValueString(42, "Model");
        Image_1.addInputArc (49, WebOptions_6, 1);


        WebOptionsNode WebOptions_7 = new WebOptionsNode (this.network, "WebOptions", 7, "WebOptions");
        this.network.addElement((Object)WebOptions_7);


        ImageNode Image_2 = new ImageNode (this.network, "Image", 2, "Image");
        this.network.addElement((Object)Image_2);
        Image_2.addInputArc (49, WebOptions_7, 1);
    }
}
