AC_INIT([pars2tab], [1.0.0], [glotfeltyk@si.edu])
AM_INIT_AUTOMAKE([foreign -Wall])
AC_PROG_CXX

export PKG_CONFIG_PATH=$prefix/lib/pkgconfig:$prefix/ots/lib/pkgconfig

PKG_CONFIG_PATH=./lib/pkgconfig:${PKG_CONFIG_PATH}

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
  boiler/Makefile
])

AC_CONFIG_FILES([boiler/pars2tab:boiler/wrapper.sh],[chmod +x boiler/pars2tab])

AC_OUTPUT
  
