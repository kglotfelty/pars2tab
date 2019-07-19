AC_INIT([pars2tab], [1.0.0], [glotfeltyk@si.edu])
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_CXX
PKG_CHECK_MODULES( [CIAO], [
  ascdm
  cxcparam
  stk
  err
  ds
])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
  
