## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/INTERFACE_CONTROL

LINKLIST = \
         DialogStyle.cfg\
         DialogStyle.net\
         InterfaceControl1.cfg\
         InterfaceControl1.net\
         InterfaceControl2.cfg\
         InterfaceControl2.net\
         InterfaceControl3.cfg\
         InterfaceControl3.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
