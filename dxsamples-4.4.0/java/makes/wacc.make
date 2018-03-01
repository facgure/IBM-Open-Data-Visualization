OBJS = \
	wacc.class

.waccpage: $(OBJS)
	make -f wacc.make $(OBJS)
	touch .waccpage

wacc.class: wacc.java
	javac $(JFLAGS) wacc.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .waccpage
	cp -f wacc.class $(JAVADIR)
	cp -f wacc.html $(JAVADIR)
	cp -f wacc1.0.* $(JAVADIR)
	cp -f wacc.net $(DXSERVER_DIR)


clean:
	rm -f .waccpage
	rm -f $(DXSERVER_DIR)/wacc.net
	rm -f $(JAVADIR)/wacc.class
	rm -f $(JAVADIR)/wacc.html
	rm -f $(JAVADIR)/wacc1.*
