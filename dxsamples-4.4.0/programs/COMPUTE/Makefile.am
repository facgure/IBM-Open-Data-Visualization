## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/COMPUTE

LINKLIST = \
         Bounce.cfg\
         Bounce.net\
         Compute2.cfg\
         Compute2.net\
         ComputeMultiLine.cfg\
         ComputeMultiLine.net\
         ComputeOnData.cfg\
         ComputeOnData.net\
         WarpingPositions.cfg\
         WarpingPositions.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
