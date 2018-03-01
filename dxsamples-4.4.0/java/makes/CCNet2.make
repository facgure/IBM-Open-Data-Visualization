##
## There is currently no way to set these values inside the ui.
## You can edit in appropriate values or you can enter them
## in $HOME/DX as follows:
## 	DX*cosmoDir: /some/where/vrml
## 	DX*dxJarFile: /some/where/java/htmlpages/dx.jar
## 	DX*jdkDir: /usr/jdk_base/lib/classes.zip
## Note that a value for cosmoDir is required if your
## web page uses vrml and may be left out otherwise.
##
JARFILE=../../java/htmlpages/dx.jar
JDKFILE=/usr/jdk_base/lib/classes.zip
COSMO=/usr/svs/vrml
JFLAGS=-classpath $(JDKFILE):$(JARFILE):$(COSMO)

OBJS = \
	CCNet2.class

.CCNet2page: $(OBJS)
	make -f CCNet2.make $(OBJS)
	touch .CCNet2page

CCNet2.class: CCNet2.java
	javac $(JFLAGS) CCNet2.java



##
## There is currently no way to set these values inside the ui.
## You can hand edit in appropriate values or you can enter them
## in $HOME/DX as follows:
## 	DX*htmlDir: /usr/http/java/htmlpages
## 	DX*serverDir: /usr/admin/java/server
##
DXSERVER=../../java/server
DXSERVER_DIR=$(DXSERVER)/unixnets
JAVADIR=../../java/htmlpages

##
## This is the install rule.  You might add lines to copy macros
## to $(DXSERVER)/usermacros or data to $(DXSERVER)/userdata
##
install: .CCNet2page
	cp -f CCNet2.class $(JAVADIR)
	cp -f Norm.dx $(DXSERVER)/userdata
	cp -f CCAll.dx $(DXSERVER)/userdata
	cp -f CCNet2.html $(JAVADIR)
	cp -f CCNet23.0.* $(JAVADIR)
	cp -f CCNet2.net $(DXSERVER_DIR)


clean:
	rm -f .CCNet2page
	rm -f $(DXSERVER_DIR)/CCNet2.net
	rm -f $(JAVADIR)/CCNet2.class
	rm -f $(DXSERVER)/userdata/Norm.dx
	rm -f $(DXSERVER)/userdata/CCAll.dx
	rm -f $(JAVADIR)/CCNet2.html
	rm -f $(JAVADIR)/CCNet23.*
