## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/CATEGORICAL

LINKLIST = \
         Categorical.cfg\
         Categorical.net\
         Duplicates.cfg\
         Duplicates.net\
         ExamplePieChart.cfg\
         ExamplePieChart.net\
         GroceryList.cfg\
         GroceryList.net\
         Legend.cfg\
         Legend.net\
         SalesOnStates.cfg\
         SalesOnStates.net\
         ZipCodes.cfg\
         ZipCodes.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
