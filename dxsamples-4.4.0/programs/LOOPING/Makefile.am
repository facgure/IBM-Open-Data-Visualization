## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/LOOPING

LINKLIST = \
         Accumulate.cfg\
         Accumulate.net\
         Bounce.cfg\
         Bounce.net\
         SimpleGetSetLoop.cfg\
         SimpleGetSetLoop.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
