## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/PLOTTING

LINKLIST = \
         Categorical.cfg\
         Categorical.net\
         GroceryList.cfg\
         GroceryList.net\
         Histogram.cfg\
         Histogram.net\
         InterfaceControl2.cfg\
         InterfaceControl2.net\
         PickPlot.cfg\
         PickPlot.net\
         PlotGroupOfLines.cfg\
         PlotGroupOfLines.net\
         PlotLine.cfg\
         PlotLine.net\
         PlotLine2.cfg\
         PlotLine2.net\
         PlotSpecifyTicks.cfg\
         PlotSpecifyTicks.net\
         PlotTwoLines.cfg\
         PlotTwoLines.net\
         Sort.cfg\
         Sort.net\
         UsingBarChart.cfg\
         UsingBarChart.net\
         UsingEqualize.cfg\
         UsingEqualize.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
