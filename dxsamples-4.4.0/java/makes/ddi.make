OBJS = \
	ddi.class

.ddipage: $(OBJS)
	make -f ddi.make $(OBJS)
	touch .ddipage

ddi.class: ddi.java
	javac $(JFLAGS) ddi.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .ddipage
	cp -f ddi.class $(JAVADIR)
	cp -f ddi.html $(JAVADIR)
	cp -f ddi5.0.* $(JAVADIR)
	cp -f ddi7.0.* $(JAVADIR)
	cp -f ddi.net $(DXSERVER_DIR)


clean:
	rm -f .ddipage
	rm -f $(DXSERVER_DIR)/ddi.net
	rm -f $(JAVADIR)/ddi.class
	rm -f $(JAVADIR)/ddi.html
	rm -f $(JAVADIR)/ddi5.*
	rm -f $(JAVADIR)/ddi7.*
