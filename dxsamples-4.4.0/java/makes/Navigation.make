OBJS = \
	Navigation.class

.Navigationpage: $(OBJS)
	make -f Navigation.make $(OBJS)
	touch .Navigationpage

Navigation.class: Navigation.java
	javac $(JFLAGS) Navigation.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .Navigationpage
	cp -f Navigation.class $(JAVADIR)
	cp -f Navigation.html $(JAVADIR)
	cp -f Navigation1.0.* $(JAVADIR)
	cp -f Navigation6.0.* $(JAVADIR)
	cp -f Navigation7.0.* $(JAVADIR)
	cp -f Navigation.net $(DXSERVER_DIR)


clean:
	rm -f .Navigationpage
	rm -f $(DXSERVER_DIR)/Navigation.net
	rm -f $(JAVADIR)/Navigation.class
	rm -f $(JAVADIR)/Navigation.html
	rm -f $(JAVADIR)/Navigation1.*
	rm -f $(JAVADIR)/Navigation6.*
	rm -f $(JAVADIR)/Navigation7.*
	rm -f $(JAVADIR)/Navigation8.*
