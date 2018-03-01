OBJS = \
	MRI_2.class

.MRI_2page: $(OBJS)
	make -f MRI_2.make $(OBJS)
	touch .MRI_2page

MRI_2.class: MRI_2.java
	javac $(JFLAGS) MRI_2.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .MRI_2page
	cp -f MRI_2.class $(JAVADIR)
	cp -f MRI_2.html $(JAVADIR)
	cp -f MRI_21.0.* $(JAVADIR)
	cp -f MRI_2.net $(DXSERVER_DIR)


clean:
	rm -f .MRI_2page
	rm -f $(DXSERVER_DIR)/MRI_2.net
	rm -f $(JAVADIR)/MRI_2.class
	rm -f $(JAVADIR)/MRI_2.html
	rm -f $(JAVADIR)/MRI_21.*
