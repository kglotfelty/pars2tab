tool = pars2tab

binexedir = $(prefix)/binexe
paramdir = $(prefix)/param
ahelpdir = $(prefix)/share/doc/xml

binexe_PROGRAMS = $(tool)

pars2tab_SOURCES = pars2tab.cc
pars2tab_CPPFLAGS = $(CIAO_CFLAGS)
pars2tab_LDADD = $(CIAO_LIBS) 

dist_param_DATA = $(tool).par
dist_ahelp_DATA = $(tool).xml

install-data-hook:
	chmod 444 $(paramdir)/$(dist_param_DATA)
	chmod 444 $(ahelpdir)/$(dist_ahelp_DATA)
