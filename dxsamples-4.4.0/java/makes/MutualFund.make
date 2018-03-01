OBJS = \
	MutualFund.class 

.MutualFundpage: $(OBJS)
	make -f MutualFund.make $(OBJS)
	touch .MutualFundpage

MutualFund.class: MutualFund.java
	javac $(JFLAGS) MutualFund.java


##
## There is currently no way to set these values inside the ui.
## Therefore you'll need to edit in values for DXSERVER_DIR and
## JAVADIR every time you generate this makefile in order for
## the install rule to work properly.
##
DXSERVER_DIR=../../java/server/unixnets
JAVADIR=../../java/htmlpages
install: .MutualFundpage
	cp -f MutualFund.class $(JAVADIR)
	cp -f MutualFund.html $(JAVADIR)
	cp -f MutualFund2.0.* $(JAVADIR)
	cp -f MutualFund3.0.* $(JAVADIR)
	cp -f MutualFund10.0.* $(JAVADIR)
	cp -f MutualFund.net $(DXSERVER_DIR)


clean:
	rm -f .MutualFundpage
	rm -f $(DXSERVER_DIR)/MutualFund.net
	rm -f $(JAVADIR)/MutualFund.class
	rm -f $(JAVADIR)/MutualFund.html
	rm -f $(JAVADIR)/MutualFund2.*
	rm -f $(JAVADIR)/MutualFund3.*
	rm -f $(JAVADIR)/MutualFund10.*
