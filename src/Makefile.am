tool = pars2tab

binexedir = $(prefix)/binexe
paramdir = $(prefix)/param
ahelpdir = $(prefix)/share/doc/xml

binexe_PROGRAMS = $(tool)

pars2tab_SOURCES = pars2tab.cc
pars2tab_CPPFLAGS = $(CIAO_CFLAGS)
pars2tab_LDADD = $(CIAO_LIBS) $(LIBICONV)
pars2tab_LINK = $(CXX) -o $@ -Wl,-rpath,$(prefix)/lib -Wl,-rpath,$(prefix)/ots/lib 

# problems with libstdc++ and cxcparam on Linux :(
if LINUX
pars2tab_LDADD += -L$(prefix)/ots/lib -lstdc++
endif


dist_param_DATA = $(tool).par
dist_ahelp_DATA = $(tool).xml

install-data-hook:
	chmod a-w $(paramdir)/$(dist_param_DATA)
	chmod a-w $(ahelpdir)/$(dist_ahelp_DATA)
