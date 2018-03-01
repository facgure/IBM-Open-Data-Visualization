OBJS = \
	gordon.class

.gordonpage: $(OBJS)
	make -f gordon.make $(OBJS)
	touch .gordonpage

gordon.class: gordon.java
	javac $(JFLAGS) gordon.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .gordonpage
	cp -f gordon.class $(JAVADIR)
	cp -f gordon.html $(JAVADIR)
	cp -f gordon1.0.* $(JAVADIR)
	cp -f gordon2.0.* $(JAVADIR)
	cp -f gordon.net $(DXSERVER_DIR)


clean:
	rm -f .gordonpage
	rm -f $(DXSERVER_DIR)/gordon.net
	rm -f $(JAVADIR)/gordon.class
	rm -f $(JAVADIR)/gordon.html
	rm -f $(JAVADIR)/gordon1.*
	rm -f $(JAVADIR)/gordon2.*
