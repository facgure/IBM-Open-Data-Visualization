##
## There is currently no way to set these values inside the ui.
## You can edit in appropriate values or you can enter them
## in $HOME/DX as follows:
## 	DX*cosmoDir: /some/where/vrml
## 	DX*dxJarFile: /some/where/java/htmlpages/dx.jar
## 	DX*userHtmlDir: /some/where/java/user
## 	DX*userJarFile: /some/where/java/user/npvcall.jar
## 	DX*jdkDir: /usr/jdk_base/lib/classes.zip
## Note that a value for cosmoDir is required if your
## web page uses vrml and may be left out otherwise.
##
JARFILE=/u/c1tignor/ibm6000/java/htmlpages/dx.jar
JDKFILE=/usr/jdk_base/lib/classes.zip
COSMO=/usr/svs/vrml
JFLAGS=-classpath $(JDKFILE):$(JARFILE):$(COSMO)

JARS = \
	npvcall.jar

OBJS = \
	npvcall.class

.npvcallpage: $(JARS)
	make -f /a/raster/homes/raster/c1tignor/ibm6000/samples/java/npvcall.make $(OBJS)
	touch .npvcallpage

npvcall.jar: $(OBJS)
	jar cf npvcall.jar $(OBJS)

npvcall.class: npvcall.java
	javac $(JFLAGS) npvcall.java



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

##
## This is the install rule.  You might add lines to copy macros
## to $(DXSERVER)/usermacros or data to $(DXSERVER)/userdata
##
install: .npvcallpage
	cp -f npvcall.jar $(JAVADIR)
	cp -f npvcall.html $(JAVADIR)
	cp -f npvcall3.0.* $(JAVADIR)
	cp -f npvcall.net $(DXSERVER_DIR)


clean:
	rm -f .npvcallpage
	rm -f $(DXSERVER_DIR)/npvcall.net
	rm -f $(JAVADIR)/npvcall.class
	rm -f $(JAVADIR)/npvcall.html
	rm -f $(JAVADIR)/npvcall3.*
