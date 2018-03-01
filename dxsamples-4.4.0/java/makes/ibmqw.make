##
## There is currently no way to set these values inside the ui.
## You can edit in appropriate values or you can enter them
## in $HOME/DX as follows:
## 	DX*cosmoDir: /some/where/vrml
## 	DX*dxJarFile: /some/where/java/htmlpages/dx.jar
## 	DX*userHtmlDir: /some/where/java/user
## 	DX*userJarFile: /some/where/java/user/ibmqw.jar
## 	DX*jdkDir: /usr/jdk_base/lib/classes.zip
## Note that a value for cosmoDir is required if your
## web page uses vrml and may be left out otherwise.
##
JARFILE=/u/c1tignor/ibm6000/java/htmlpages/dx.jar
JDKFILE=/usr/jdk_base/lib/classes.zip
COSMO=/usr/svs/vrml
JFLAGS=-classpath $(JDKFILE):$(JARFILE):$(COSMO)

JARS = \
	ibmqw.jar

OBJS = \
	ibmqw.class

.ibmqwpage: $(JARS)
	make -f /a/raster/homes/raster/c1tignor/ibm6000/samples/java/ibmqw.make $(OBJS)
	touch .ibmqwpage

ibmqw.jar: $(OBJS)
	jar cf ibmqw.jar $(OBJS)

ibmqw.class: ibmqw.java
	javac $(JFLAGS) ibmqw.java



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
install: .ibmqwpage
	cp -f ibmqw.jar $(JAVADIR)
	cp -f ibmqw.html $(JAVADIR)
	cp -f ibmqw1.0.* $(JAVADIR)
	cp -f ibmqw2.0.* $(JAVADIR)
	cp -f ibmqw.net $(DXSERVER_DIR)


clean:
	rm -f .ibmqwpage
	rm -f $(DXSERVER_DIR)/ibmqw.net
	rm -f $(JAVADIR)/ibmqw.class
	rm -f $(JAVADIR)/ibmqw.html
	rm -f $(JAVADIR)/ibmqw1.*
	rm -f $(JAVADIR)/ibmqw2.*
