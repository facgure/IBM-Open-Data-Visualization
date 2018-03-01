## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/IMPORTING

LINKLIST = \
         Categorical.cfg\
         Categorical.net\
         Duplicates.cfg\
         Duplicates.net\
         GeneralImport1.cfg\
         GeneralImport1.net\
         GeneralImport2.cfg\
         GeneralImport2.net\
         ImportExternalFilter.cfg\
         ImportExternalFilter.net\
         ZipCodes.cfg\
         ZipCodes.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
