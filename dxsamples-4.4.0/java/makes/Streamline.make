OBJS = \
	Streamline.class

.Streamlinepage: $(OBJS)
	make -f Streamline.make $(OBJS)
	touch .Streamlinepage

Streamline.class: Streamline.java
	javac $(JFLAGS) Streamline.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .Streamlinepage
	cp -f Streamline.class $(JAVADIR)
	cp -f Streamline.html $(JAVADIR)
	cp -f Streamline2.0.* $(JAVADIR)
	cp -f Streamline7.0.* $(JAVADIR)
	cp -f Streamline.net $(DXSERVER_DIR)


clean:
	rm -f .Streamlinepage
	rm -f $(DXSERVER_DIR)/Streamline.net
	rm -f $(JAVADIR)/Streamline.class
	rm -f $(JAVADIR)/Streamline.html
	rm -f $(JAVADIR)/Streamline2.*
	rm -f $(JAVADIR)/Streamline7.*
