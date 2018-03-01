## Process this file with automake to produce Makefile.in

sampleprogdir = $(prefix)/dx/samples/programs/MISC

LINKLIST = \
         CensusData.cfg\
         CensusData.net\
         ExampleSMP.cfg\
         ExampleSMP.net\
         Factorial.cfg\
         Factorial.net\
         HomeOwn.cfg\
         HomeOwn.net\
         ImageTool.cfg\
         ImageTool.net\
         Image_wo_UI.cfg\
         Image_wo_UI.net\
         IndependentlyArrange.cfg\
         IndependentlyArrange.net\
         InsetImage.cfg\
         InsetImage.net\
         Interop.cfg\
         Interop.net\
         InvalidData.cfg\
         InvalidData.net\
         MRI_1.cfg\
         MRI_1.net\
         MRI_2.cfg\
         MRI_2.net\
         Majority.cfg\
         Majority.net\
         ManipulateGroups.cfg\
         ManipulateGroups.net\
         MultipleDataSets.cfg\
         MultipleDataSets.net\
         PickStreamline.cfg\
         PickStreamline.net\
         ScatterData.cfg\
         ScatterData.net\
         Supervise.cfg\
         Supervise.net\
         Topo.cfg\
         Topo.net\
         UsingAttributes.cfg\
         UsingAttributes.net\
         UsingDrape.cfg\
         UsingDrape.net\
         UsingEqualize.cfg\
         UsingEqualize.net\
         UsingMessage.cfg\
         UsingMessage.net\
         UsingMultiGrids.cfg\
         UsingMultiGrids.net\
         UsingParse.cfg\
         UsingParse.net\
         UsingSwitchAndRoute.cfg\
         UsingSwitchAndRoute.net

myinstall: 
	$(mkinstalldirs) $(sampleprogdir)
	@-sh -c '( echo "cd $(sampleprogdir)" ; \
		cd $(sampleprogdir) && for s in $(LINKLIST) ; do \
		$(LN_S) ../$$s $$s ; \
		echo "$(LN_S) ../$$s $$s" ; \
	done )'
