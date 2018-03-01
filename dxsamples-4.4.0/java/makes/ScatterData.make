OBJS = \
	ScatterData.class

.ScatterDatapage: $(OBJS)
	make -f ScatterData.make $(OBJS)
	touch .ScatterDatapage

ScatterData.class: ScatterData.java
	javac $(JFLAGS) ScatterData.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .ScatterDatapage
	cp -f ScatterData.class $(JAVADIR)
	cp -f ScatterData.html $(JAVADIR)
	cp -f ScatterData3.0.* $(JAVADIR)
	cp -f ScatterData.net $(DXSERVER_DIR)


clean:
	rm -f .ScatterDatapage
	rm -f $(DXSERVER_DIR)/ScatterData.net
	rm -f $(JAVADIR)/ScatterData.class
	rm -f $(JAVADIR)/ScatterData.html
	rm -f $(JAVADIR)/ScatterData3.*
