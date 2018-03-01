OBJS = \
	HomeOwn.class

.HomeOwnpage: $(OBJS)
	make -f HomeOwn.make $(OBJS)
	touch .HomeOwnpage

HomeOwn.class: HomeOwn.java
	javac $(JFLAGS) HomeOwn.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .HomeOwnpage
	cp -f HomeOwn.class $(JAVADIR)
	cp -f HomeOwn.html $(JAVADIR)
	cp -f HomeOwn1.0.* $(JAVADIR)
	cp -f HomeOwn2.0.* $(JAVADIR)
	cp -f HomeOwn3.0.* $(JAVADIR)
	cp -f HomeOwn.net $(DXSERVER_DIR)


clean:
	rm -f .HomeOwnpage
	rm -f $(DXSERVER_DIR)/HomeOwn.net
	rm -f $(JAVADIR)/HomeOwn.class
	rm -f $(JAVADIR)/HomeOwn.html
	rm -f $(JAVADIR)/HomeOwn1.*
	rm -f $(JAVADIR)/HomeOwn2.*
	rm -f $(JAVADIR)/HomeOwn3.*
