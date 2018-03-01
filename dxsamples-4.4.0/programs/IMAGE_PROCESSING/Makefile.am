## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/IMAGE_PROCESSING

LINKLIST = \
         FFT.cfg\
         FFT.net\
         MRI_1.cfg\
         MRI_1.net\
         QuantizeImage.cfg\
         QuantizeImage.net\
         UsingEqualize.cfg\
         UsingEqualize.net\
         UsingFilter.cfg\
         UsingFilter.net\
         UsingMorph.cfg\
         UsingMorph.net\
         UsingOverlay.cfg\
         UsingOverlay.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
