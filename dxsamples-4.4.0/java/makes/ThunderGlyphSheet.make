OBJS = \
	ThunderGlyphSheet.class

.ThunderGlyphSheetpage: $(OBJS)
	make -f ThunderGlyphSheet.make $(OBJS)
	touch .ThunderGlyphSheetpage

ThunderGlyphSheet.class: ThunderGlyphSheet.java
	javac $(JFLAGS) ThunderGlyphSheet.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .ThunderGlyphSheetpage
	cp -f ThunderGlyphSheet.class $(JAVADIR)
	cp -f ThunderGlyphSheet.html $(JAVADIR)
	cp -f ThunderGlyphSheet.htm $(JAVADIR)
	cp -f ThunderGlyphSheet2.0.* $(JAVADIR)
	cp -f ThunderGlyphSheet3.0.* $(JAVADIR)
	cp -f ThunderGlyphSheet.net $(DXSERVER_DIR)


clean:
	rm -f .ThunderGlyphSheetpage
	rm -f $(DXSERVER_DIR)/ThunderGlyphSheet.net
	rm -f $(JAVADIR)/ThunderGlyphSheet.class
	rm -f $(JAVADIR)/ThunderGlyphSheet.html
	rm -f $(JAVADIR)/ThunderGlyphSheet.htm
	rm -f $(JAVADIR)/ThunderGlyphSheet2.*
	rm -f $(JAVADIR)/ThunderGlyphSheet3.*
