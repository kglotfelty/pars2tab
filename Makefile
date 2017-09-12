#-----------------------------------------------------------------------
#
#	Makefile for the par2html program C version
#
#-----------------------------------------------------------------------

MK_TOP = /export/ciao_from_source/ciao-4.8/src
KJG = /export/ciao


include $(MK_TOP)/Makefile.master
include $(MK_TOP)/include/Makefile.scidev


EXEC              = pars2tab
LIB_FILES         = 
PAR_FILES         = pars2tab.par
INC_FILES         =
XML_FILES         = pars2tab.xml

SRCS	= pars2tab.cc

OBJS	= $(SRCS:.cc=.o)


MAKETEST_SCRIPT=pars2tab.t


include $(MK_TOP)/Makefile.all

#-----------------------------------------------------------------------
# 			MAKEFILE DEPENDENCIES	
#-----------------------------------------------------------------------
$(EXEC): $(OBJS) 
	$(LINK)
	@echo

kjg: $(EXEC)
	/bin/cp -f $(EXEC) $(KJG)/binexe/
	/bin/cp -f $(KJG)/bin/dmlist $(KJG)/bin/$(EXEC)
	/bin/cp -f $(PAR_FILES) $(KJG)/param/$(PAR_FILES)




announce1:
	@echo "   /----------------------------------------------------------\ "
	@echo "   |            Building pars2tab program                     | "
	@echo "   \----------------------------------------------------------/ "


# DO NOT DELETE THIS LINE -- make depend depends on it.
