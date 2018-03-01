## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/SUPERVISE

LINKLIST = \
         Image_wo_UI.cfg\
         Image_wo_UI.net\
         IndependentlyArrange.cfg\
         IndependentlyArrange.net\
         InsetImage.cfg\
         InsetImage.net\
         InsetImageSimple.cfg\
         InsetImageSimple.net\
         Supervise.cfg\
         Supervise.net\
         SupervisePicking.cfg\
         SupervisePicking.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
