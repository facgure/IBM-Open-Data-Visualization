OBJS = \
	Topo.class

.Topopage: $(OBJS)
	make -f Topo.make $(OBJS)
	touch .Topopage

Topo.class: Topo.java
	javac $(JFLAGS) Topo.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .Topopage
	cp -f Topo.class $(JAVADIR)
	cp -f Topo.html $(JAVADIR)
	cp -f Topo0.0.* $(JAVADIR)
	cp -f Topo2.0.* $(JAVADIR)
	cp -f Topo4.0.* $(JAVADIR)
	cp -f Topo.net $(DXSERVER_DIR)


clean:
	rm -f .Topopage
	rm -f $(DXSERVER_DIR)/Topo.net
	rm -f $(JAVADIR)/Topo.class
	rm -f $(JAVADIR)/Topo.html
	rm -f $(JAVADIR)/Topo0.*
	rm -f $(JAVADIR)/Topo2.*
	rm -f $(JAVADIR)/Topo4.*
