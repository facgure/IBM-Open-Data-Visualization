OBJS = \
	GroceryList.class

.GroceryListpage: $(OBJS)
	make -f GroceryList.make $(OBJS)
	touch .GroceryListpage

GroceryList.class: GroceryList.java
	javac $(JFLAGS) GroceryList.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .GroceryListpage
	cp -f GroceryList.class $(JAVADIR)
	cp -f GroceryList.html $(JAVADIR)
	cp -f GroceryList2.0.* $(JAVADIR)
	cp -f GroceryList3.0.* $(JAVADIR)
	cp -f GroceryList4.0.* $(JAVADIR)
	cp -f GroceryList.net $(DXSERVER_DIR)


clean:
	rm -f .GroceryListpage
	rm -f $(DXSERVER_DIR)/GroceryList.net
	rm -f $(JAVADIR)/GroceryList.class
	rm -f $(JAVADIR)/GroceryList.html
	rm -f $(JAVADIR)/GroceryList2.*
	rm -f $(JAVADIR)/GroceryList3.*
	rm -f $(JAVADIR)/GroceryList4.*
