## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/ANNOTATION

LINKLIST = \
         AnnotationGlyphs.cfg\
         AnnotationGlyphs.net\
         AutoAxesSpecifyTicks.cfg\
         AutoAxesSpecifyTicks.net\
         BandedColors.cfg\
         BandedColors.net\
         Categorical.cfg\
         Categorical.net\
         ContoursAndCaption.cfg\
         ContoursAndCaption.net\
         Distributed.cfg\
         Distributed.net\
         ExpandedFonts.cfg\
         ExpandedFonts.net\
         FontLayouts.cfg\
         FontLayouts.net\
         FontPreview.cfg\
         FontPreview.net\
         GroceryList.cfg\
         GroceryList.net\
         Imide_potential.cfg\
         Imide_potential.net\
         Legend.cfg\
         Legend.net\
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
         ProbeText.cfg\
         ProbeText.net\
         SalesOnStates.cfg\
         SalesOnStates.net\
         ScalingAxes.cfg\
         ScalingAxes.net\
         ShowFonts.cfg\
         ShowFonts.net\
         Sort.cfg\
         Sort.net\
         SpecialCharacters.cfg\
         SpecialCharacters.net\
         ThunderGlyphSheet.cfg\
         ThunderGlyphSheet.net\
         UsingGlyphs.cfg\
         UsingGlyphs.net\
         UsingTextAndTextGlyphs.cfg\
         UsingTextAndTextGlyphs.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
