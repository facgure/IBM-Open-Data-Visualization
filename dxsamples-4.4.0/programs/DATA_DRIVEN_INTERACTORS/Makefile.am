## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/DATA_DRIVEN_INTERACTORS

LINKLIST = \
         DataDrivenInteractors.cfg\
         DataDrivenInteractors.net\
         DataDrivenSelector.cfg\
         DataDrivenSelector.net\
         Imide_potential.cfg\
         Imide_potential.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
