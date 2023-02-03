#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

VERSION=`cat src/version.h | grep "#define VERSION " | sed -e 's/^.*\"\(.*\)\"/\1/'`

AC_INIT([normie], m4_esyscmd_s([cat src/version.h | cut -d\" -f2]))
AC_CONFIG_SRCDIR([src])
AC_CANONICAL_SYSTEM
AC_GNU_SOURCE
AM_MAINTAINER_MODE([disable])

# Check OS

AC_MSG_CHECKING([host os])

# If no host OS,  try uname

if test -z "$host" ; then
        host="`uname`"
fi

echo -n "installation for $host OS... "

    case "$host" in
        *-*-*freebsd*)
            CFLAGS="${CFLAGS} -D__FreeBSD__"
            CPPFLAGS="${CPPFLAGS} -I/usr/local/include -I/usr/local/include/libnet11"
            LDFLAGS="${LDFLAGS} -L/usr/local/lib -L/usr/local/lib/libnet11"
            ;;
        *-*-openbsd*)
            CFLAGS="${CFLAGS} -D__OpenBSD__"
            CPPFLAGS="${CPPFLAGS} -I/usr/local/include -I/usr/local/include/libnet-1.1"
            LDFLAGS="${LDFLAGS} -L/usr/local/lib -I/usr/local/lib/libnet-1.1"
            ;;
        *darwin*|*Darwin*)
            CFLAGS="${CFLAGS} -D__Darwin__"
            CPPFLAGS="${CPPFLAGS} -I/opt/local/include"
            LDFLAGS="${LDFLAGS} -L/opt/local/lib"
            ;;
        *-*-linux*)
	    CFLAGS="${CFLAGS} -D__Linux__"
            #for now do nothing
            ;;
#        *-*-mingw32*)
#            CFLAGS="${CFLAGS} -DOS_WIN32"
#            LDFLAGS="${LDFLAGS} -lws2_32"
#            WINDOWS_PATH="yes"
#            ;;
#        *-*-cygwin)
#            WINDOWS_PATH="yes"
#            ;;
#        *-*-solaris*)
#            AC_MSG_WARN([support for Solaris/Illumos/SunOS is experimental])
#            LDFLAGS="${LDFLAGS} -lsocket -lnsl"
#            ;;
        *)
            AC_MSG_WARN([unsupported OS this may or may not work])
            ;;
    esac
    AC_MSG_RESULT(ok)

AC_ARG_ENABLE(tcmalloc,
  [  --enable-tcmalloc       Enable TCMalloc support.],
  [ TCMALLOC="$enableval"],
  [ TCMALLOC="no" ]
)

AC_ARG_ENABLE(jemalloc,
  [  --enable-jemalloc       Enable Jemalloc support.],
  [ JEMALLOC="$enableval"],
  [ JEMALLOC="no" ]
)

AC_ARG_WITH(lognorm_includes,
        [  --with-lognorm-includes=DIR    liblognorm include directory],
        [with_lognorm_includes="$withval"],[with_lognorm_includes="no"])

AC_ARG_WITH(lognorm_libraries,
        [  --with-lognorm-libraries=DIR   liblognorm library directory],
        [with_lognorm_libraries="$withval"],[with_lognorm_libraries="no"])

if test "x$with_lognorm_includes" != "xno"; then
   CPPFLAGS="${CPPFLAGS} -I${with_lognorm_includes}"
   fi

if test "x$with_lognorm_libraries" != "xno"; then
   LDFLAGS="${LDFLAGS}  -L${with_lognorm_libraries}"
fi

AC_ARG_WITH(libfastjson_includes,
        [  --with-libfastjson-includes=DIR    libfastjson include directory],
        [with_libfastjson_includes="$withval"],[with_libfastjson_includes="no"])

AC_ARG_WITH(libfastjson_c_libraries,
        [  --with-libfastjson-libraries=DIR   libfastjson library directory],
        [with_libfastjson_libraries="$withval"],[with_libfastjson_libraries="no"])

if test "x$with_libfastjson_includes" != "xno"; then
   CPPFLAGS="${CPPFLAGS} -I${with_libfastjson_includes}"
   fi

if test "x$with_libfastjson_libraries" != "xno"; then
   LDFLAGS="${LDFLAGS}  -L${with_libfastjson_libraries}"
fi


AC_SUBST(VERSION)
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_LANG_C
AC_PROG_MAKE_SET

AM_PROG_CC_C_O

PKG_PROG_PKG_CONFIG

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT

AC_CHECK_HEADERS([stdio.h sys/stat.h string.h])

AC_CHECK_SIZEOF([size_t])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_TIME

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_TYPE_SIGNAL
AC_FUNC_STRFTIME
AC_FUNC_FORK
AC_FUNC_REALLOC
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_TM 

AM_PROG_AS

AC_CHECK_FUNCS([strlen strlcpy])

if test "$TCMALLOC" = "yes"; then
       AC_MSG_RESULT([------- TCMalloc support is enabled -------])
       AC_CHECK_LIB(tcmalloc_minimal,main,,AC_MSG_ERROR(The libtcmalloc_minimal library cannot be found.))
       fi

if test "$JEMALLOC" = "yes"; then
       AC_MSG_RESULT([------- Jemalloc support is enabled -------])
       AC_CHECK_LIB(jemalloc,main,,AC_MSG_ERROR(The libjemalloc library cannot be found.))
       fi

       AC_MSG_RESULT([------- liblognorm found -------])
       PKG_CHECK_MODULES(LIBESTR, libestr >= 0.0.0)
       PKG_CHECK_MODULES(LIBLOGNORM, lognorm >= 1.0.0)
       PKG_CHECK_MODULES(LIBFASTJSON, libfastjson >= 0.0.0)
       AC_CHECK_HEADER([liblognorm.h])
       AC_CHECK_HEADER([libestr.h])
       AC_CHECK_LIB(fastjson, main,,AC_MSG_ERROR(The libfastjson library cannot be found.
This library is used by liblognorm for log correlation.  Please see:
https://wiki.quadrantsec.com/bin/view/Main/LibLogNorm. To disable this feature
use the --disable-lognorm flag. 
This library can be located at https://github.com/rsyslog/libfastjson. ))
       AC_CHECK_LIB(estr, main,,AC_MSG_ERROR(The libestr library cannot be found.
This library is important for log correlation. Please see:
https://wiki.quadrantsec.com/bin/view/Main/LibLogNorm. To disable this feature
use the --disable-lognorm flag. ))
      AC_CHECK_LIB(lognorm, main,,AC_MSG_ERROR(The liblognorm library cannot be found.
This library is important for log correlation. Please see:
https://wiki.quadrantsec.com/bin/view/Main/LibLogNorm. To disable this feature
use the --disable-lognorm flag. ))

AC_DEFINE_UNQUOTED(PACKAGE_NAME, "normie" )
AC_DEFINE_UNQUOTED(PACKAGE_STRING, "normie $VERSION")
AC_DEFINE_UNQUOTED(PACKAGE_BUGREPORT, "cclark@quadrantsec.com" )
AC_DEFINE_UNQUOTED(PACKAGE_TARNAME, "normie" )
AC_DEFINE_UNQUOTED(PACKAGE_VERSION, "$VERSION" )

AC_CONFIG_FILES([ \
Makefile \
src/Makefile])


# AC_OUTPUT(Makefile)
AC_OUTPUT

AC_MSG_RESULT([])
AC_MSG_RESULT([Normie has been configured!])


