##
## There is currently no way to set these values inside the ui.
## You can edit in appropriate values or you can enter them
## in $HOME/DX as follows:
## 	DX*cosmoDir: /some/where/vrml
## 	DX*dxJarFile: /some/where/java/htmlpages/dx.jar
## 	DX*userHtmlDir: /some/where/java/user
## 	DX*userJarFile: /some/where/java/user/hedge.jar
## 	DX*jdkDir: /usr/jdk_base/lib/classes.zip
## Note that a value for cosmoDir is required if your
## web page uses vrml and may be left out otherwise.
##
JARFILE=/u/c1tignor/ibm6000/java/htmlpages/dx.jar
JDKFILE=/usr/jdk_base/lib/classes.zip
COSMO=/usr/svs/vrml
JFLAGS=-classpath $(JDKFILE):$(JARFILE):$(COSMO)

JARS = \
	hedge.jar

OBJS = \
	hedge.class

.hedgepage: $(JARS)
	make -f /a/raster/homes/raster/c1tignor/options/hedge.make $(OBJS)
	touch .hedgepage

hedge.jar: $(OBJS)
	jar cf hedge.jar $(OBJS)

hedge.class: hedge.java
	javac $(JFLAGS) hedge.java



##
## There is currently no way to set these values inside the ui.
## You can hand edit in appropriate values or you can enter them
## in $HOME/DX as follows:
## 	DX*htmlDir: /usr/http/java
## 	DX*userHtmlDir: user
## 	DX*serverDir: /usr/admin/java/server
##
DXSERVER=/u/c1tignor/ibm6000/java/server
DXSERVER_DIR=$(DXSERVER)/unixnets
JAVADIR=/u/c1tignor/ibm6000/java/user
MACROS = bschols.net logn.net optsurf.net pdflogn.net

##
## This is the install rule.  You might add lines to copy macros
## to $(DXSERVER)/usermacros or data to $(DXSERVER)/userdata
##
install: .hedgepage
	cp -f hedge.jar $(JAVADIR)
	cp -f hedge.html $(JAVADIR)
	cp -f hedge3.0.* $(JAVADIR)
	cp -f hedge.net $(DXSERVER_DIR)
	cp -f ../demos/option.cm $(DXSERVER)/userdata
	cp -f $(MACROS) $(DXSERVER)/usermacros



clean:
	rm -f .hedgepage
	rm -f $(DXSERVER_DIR)/hedge.net
	rm -f $(JAVADIR)/hedge.class
	rm -f $(JAVADIR)/hedge.html
	rm -f $(JAVADIR)/hedge3.*
