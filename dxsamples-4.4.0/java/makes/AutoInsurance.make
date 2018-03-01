OBJS = \
	AutoInsurance.class

.AutoInsurancepage: $(OBJS)
	make -f AutoInsurance.make $(OBJS)
	touch .AutoInsurancepage

AutoInsurance.class: AutoInsurance.java
	javac $(JFLAGS) AutoInsurance.java



##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .AutoInsurancepage
	cp -f AutoInsurance.class $(JAVADIR)
	cp -f AutoInsurance.html $(JAVADIR)
	cp -f AutoInsurance1.0.* $(JAVADIR)
	cp -f AutoInsurance2.0.* $(JAVADIR)
	cp -f AutoInsurance.net $(DXSERVER_DIR)


clean:
	rm -f .AutoInsurancepage
	rm -f $(DXSERVER_DIR)/AutoInsurance.net
	rm -f $(JAVADIR)/AutoInsurance.class
	rm -f $(JAVADIR)/AutoInsurance.html
	rm -f $(JAVADIR)/AutoInsurance1.*
	rm -f $(JAVADIR)/AutoInsurance2.*
