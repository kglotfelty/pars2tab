binexedir = $(prefix)/binexe
paramdir = $(prefix)/param
ahelpdir = $(prefix)/share/doc/xml

binexe_PROGRAMS = pars2tab

pars2tab_SOURCES = pars2tab.cc
pars2tab_CPPFLAGS = $(CIAO_CFLAGS)
pars2tab_LDADD = $(CIAO_LIBS) 

dist_param_DATA = pars2tab.par
dist_ahelp_DATA = pars2tab.xml
