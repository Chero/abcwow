## Process this file with autoconf to produce a configure script.

AC_INIT([ArcEmu],
        [0.5],
        [www.arcemu.org],
        [arcemu])

AC_PREREQ(2.5)

if test -d obj; then rm -rf obj; fi

AM_CONFIG_HEADER( config.h )
INCLUDES="-I/usr/include -I/usr/local/include"
AC_SUBST(INCLUDES)
LDFLAGS="-L/usr/lib -L/usr/local/lib"
AC_SUBST(LDFLAGS)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# autoheader templates
AH_TEMPLATE([USE_EPOLL], Define if you want to use the epoll socket engine module.)
AH_TEMPLATE([USE_KQUEUE], Define if you want to use the kqueue socket engine module.)
AH_TEMPLATE([HAVE_DARWIN], Define if you have MacOSX Darwin, to include the correct headers for kevent)
AH_TEMPLATE([ENABLE_IMPROVED_TICKETSYSTEM], Define if you want to enable the improved ticket system. Warning: Not My_Master compatible!)
AH_TEMPLATE([USING_BIG_ENDIAN], Define this if you are on a big-endian system.)
AH_TEMPLATE([USING_LITTLE_ENDIAN], Define this if you are on a non-big-endian system.)
AH_TEMPLATE([X64], Define this if you are under an X64-based operating system.)
AH_TEMPLATE([X86], Define this if you are under an X86-based 32bit operating system.)
AH_TEMPLATE([NO_DBLAYER_MYSQL], Define this if you want to disable mysql support.)
AH_TEMPLATE([ENABLE_DATABASE_POSTGRES], Define this if you want postgres support.)
AH_TEMPLATE([ENABLE_DATABASE_SQLITE], Define this if you want sqlite support.)

# Default prefix
AC_PREFIX_DEFAULT(`pwd`)
test "$prefix" = "NONE" && prefix=`pwd`
test "$sysconfdir" = "\${prefix}/etc" && sysconfdir='../etc'
test "$scriptdir" = "" && scriptdir='../lib'

# Determine the operating system
AC_MSG_CHECKING([uname -s for detecting host operating system])
OSNAME=`uname -s`
case "$OSNAME" in
	FreeBSD*)
		dnl FreeBSD Owns !
		AC_MSG_RESULT(FreeBSD Owns your hd)
		FREEBSD=yes
	;;
	Linux*)
		dnl Linux...
		AC_MSG_RESULT(Linux)
		LINUX=yes
	;;
	Darwin*)
		dnl Darwin owns to (includes BSD) :P
		AC_MSG_RESULT(Darwin)
		FREEBSD=yes
	;;
	*)
		AC_MSG_RESULT(no)
	;;
esac

AC_MSG_CHECKING([uname -m for detecting host operating system architecture])
ARCH=`uname -m`
case "$ARCH" in
	amd64*)
		dnl yay 21st century hardware
		AC_MSG_RESULT(x64 feel teh power)
		AC_DEFINE(X64)
		AC_SUBST(ARCH,x64)
	;;
	x86_64*)
		dnl yay 21st century hardware
                AC_MSG_RESULT(x64 feel teh power)
                AC_DEFINE(X64)
		AC_SUBST(ARCH,x64)
	;;
	*)
		AC_MSG_RESULT(x86 oh noes i ran out of address space)
		AC_DEFINE(X86)
		AC_SUBST(ARCH,x86)
	;;
esac

AC_C_BIGENDIAN([AC_DEFINE(USING_BIG_ENDIAN)],[AC_DEFINE(USING_LITTLE_ENDIAN)],[])

AC_CHECK_PROG([SVN], [svn], yes)
if test -z "$SVN"; then AC_MSG_ERROR(Subversion not found); fi

# Checks for libraries.

# Reset 
CFLAGS="$INCLUDES -Wall -fno-strict-aliasing"
CXXFLAGS="$INCLUDES -Wall -fno-strict-aliasing"

# Check for pthread
ACX_PTHREAD( [
	LDFLAGS="$PTHREAD_LIBS $LDFLAGS"
       	CXXFLAGS="$CFLAGS $PTHREAD_CFLAGS"
       	CFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
       	CC="$PTHREAD_CC"
],AC_MSG_ERROR(pthread not found) )

AC_CHECK_LIB( z, compress, [],[AC_MSG_ERROR([Missing zlib])] )
AC_CHECK_LIB( compat, ftime )

if test "x$OSNAME" = "xDarwin" ; then
	AC_CHECK_LIB( crypto, SHA1_Init, [], [AC_CHECK_LIB(ssl, SHA1_Init,[],[AC_MSG_ERROR([Missing openssl])])])
	AC_DEFINE(HAVE_DARWIN)
	LIB_EXPORT="-framework System -framework Foundation"
	AC_SUBST(LIB_EXPORT)
else
	#AC_CHECK_LIB( ssl, SHA1_Init, [], [AC_CHECK_LIB(ssl, SHA1_Init,[],[AC_MSG_ERROR([Missing openssl])])])
	CHECK_SSL(1)
	LIB_EXPORT="-Wl,--export-dynamic"
	AC_SUBST(LIB_EXPORT)
fi

AC_ARG_ENABLE(ticketsystemex,
     AC_HELP_STRING([--enable-ticketsystemex],enable improved ticket system. warning not my_master compatible),
     [ if test "x$enable_ticketsystemex" = "xyes" ; then
   	AC_DEFINE(ENABLE_IMPROVED_TICKETSYSTEM)
       fi
     ],
)

AC_ARG_ENABLE(gcov,
     AC_HELP_STRING([--enable-gcov],[enable codecovering]),
     [ if test "x$enable_gcov" = "xyes" ; then
           CXXFLAGS="$CXXFLAGS -fprofile-arcs -ftest-coverage"
       fi
     ],
)

AC_ARG_ENABLE(debug,
     AC_HELP_STRING([--disable-debug],[disable debug]),
	 [enable_debug=$enableval],
	 [enable_debug=yes]
)

if test "x$enable_debug" = "xyes" ; then
	CFLAGS="$CFLAGS -g -D__DEBUG__"
	CXXFLAGS="$CXXFLAGS -g -D__DEBUG__"
fi

AC_ARG_ENABLE(optimize,
     AC_HELP_STRING([--enable-optimize],[enable code optimization]),
	 [enable_optimize=$enableval],
	 [enable_optimize=no]
)

if test "x$enable_optimize" = "xyes" ; then
	CFLAGS="$CFLAGS -O2 -pipe"
	CXXFLAGS="$CXXFLAGS -O2 -pipe"
	if test "x$enable_debug" != "xyes" ; then
		CFLAGS="$CFLAGS -fomit-frame-pointer"
		CXXFLAGS="$CXXFLAGS -fomit-frame-pointer"
	fi
fi

AC_ARG_ENABLE(profiling,
     AC_HELP_STRING([--enable-profiling],[build for profiling using gprof.]),
     [ if test "x$enable_profiling" = "xyes" ; then
           CXXFLAGS="$CXXFLAGS -pg" ;
           #CFLAGS="$CFLAGS -pg" ;
       fi
     ],
)

AC_ARG_ENABLE(mysql,
	AC_HELP_STRING([--without-mysql],disables mysql backend),
	[ if test "x$without_mysql" = "xyes" ; then
		AC_DEFINE(NO_DBLAYER_MYSQL)
		fi
	],
)

AC_ARG_ENABLE(postgres,
	AC_HELP_STRING([--enable-postgres],enables postgres backend),
	[ if test "x$enable_postgres" = "xyes" ; then
		AC_DEFINE(ENABLE_DATABASE_POSTGRES)
		AX_LIB_POSTGRESQL
		fi
	],
)

AC_ARG_ENABLE(sqlite,
	AC_HELP_STRING([--enable-sqlite],enables sqlite backend),
	[ if test "x$enable_sqlite" = "xyes" ; then
		AC_DEFINE(ENABLE_DATABASE_SQLITE)
		fi
	],
)

# Check for networking stuff
AC_MSG_CHECKING([detecting network socket engine])
if test "x$OSNAME" = "xLinux" ; then
	AC_MSG_RESULT(epoll)
	AC_CHECK_HEADERS(sys/epoll.h)
	AC_CHECK_FUNC(epoll_ctl, ,
		[AC_CHECK_LIB(epoll, epoll_ctl, , AC_MSG_ERROR([Cannot find epoll_ctl() function for socket engine]))])
	AC_DEFINE(USE_EPOLL)
	AC_SUBST(NETWORKING, "epoll (Linux)")
fi

if test "x$OSNAME" = "xFreeBSD" ; then
	AC_MSG_RESULT(kqueue)
	AC_CHECK_FUNC(kevent, , AC_MSG_ERROR([Cannot find kevent() function for socket engine]))
	AC_CHECK_HEADER(sys/event.h, , AC_MSG_ERROR([Cannot find sys/event.h for socket engine]))
	AC_DEFINE(USE_KQUEUE)
	AC_SUBST(NETWORKING, "kqueue (FreeBSD)")
fi

if test "x$OSNAME" = "xDarwin" ; then
	AC_MSG_RESULT(kqueue)
	AC_CHECK_FUNC(kevent, , AC_MSG_ERROR([Cannot find kevent() function for socket engine]))
	AC_CHECK_HEADER(sys/event.h, , AC_MSG_ERROR([Cannot find sys/event.h for socket engine]))
	AC_DEFINE(USE_KQUEUE)
	AC_SUBST(NETWORKING, "kqueue (Darwin)")

	INCLUDES+="-I/sw/include -I/sw/include/mysql -I/usr/include/openssl"
	LDFLAGS+="-L/sw/lib -L/sw/lib/mysql -L/usr/lib"
fi

# Check for mysql
MYSQL_FOUND=AX_LIB_MYSQL([5.0])
if test "$MYSQL_VERSION" = ""; then
	AC_MSG_ERROR([*** mysql client library not found. Please install it.])
else 
	LDFLAGS="$LDFLAGS $MYSQL_LDFLAGS"
	CFLAGS="$CFLAGS $MYSQL_CFLAGS"
	CXXFLAGS="$CXXFLAGS $MYSQL_CFLAGS"
fi

# pcre 
AX_PATH_LIB_PCRE([
	LDFLAGS="$LDFLAGS $PCRE_LIBS"
	CFLAGS="$CFLAGS $PCRE_CFLAGS"
	CXXFLAGS="$CXXFLAGS $PCRE_CFLAGS"
],AC_MSG_ERROR(Couldn't find pcre library. Make sure your pcre path is correct.))



AC_DEFINE_UNQUOTED([PREFIX], "${prefix}", "Prefix")
AC_DEFINE_UNQUOTED([SCRIPTDIR], "${scriptdir}", "librarydir")
AC_DEFINE_UNQUOTED([CONFDIR], "${sysconfdir}", "configdir")

# AC_DEFINE_UNQUOTED([SVN_REVISION_DONE], `sh ./extract_svn_revision.sh`, [SVN revision generated])

AC_CONFIG_FILES([
   Makefile
   src/Makefile
   src/arcemu-shared/Makefile
   src/arcemu-world/Makefile
   src/arcemu-logonserver/Makefile
   src/scripts/Makefile
   src/scripts/src/Makefile
   src/scripts/src/GossipScripts/Makefile
   src/scripts/src/ExtraScripts/Makefile
   src/scripts/src/InstanceScripts/Makefile
   src/scripts/src/ServerStatusPlugin/Makefile
   src/scripts/src/SpellHandlers/Makefile
   src/scripts/src/LUAScripting/Makefile
   src/scripts/src/QuestScripts/Makefile
   src/scripts/src/MiscScripts/Makefile
   extras/Makefile
   extras/collision/Makefile
   extras/collision/collision_dll/Makefile
])

AC_OUTPUT

echo
echo "===================================="
echo "ArcEmu configure report:"
echo
echo " Core:" 
echo "             os: ${OSNAME}"
echo "         prefix: ${prefix}"
echo "   architecture: ${ARCH}"
echo 
echo " Database modules:"
echo "        mysql: ${MYSQL_VERSION}"
if test "x$enable_postgres" = "xyes" ; then
  echo "   postgresql: ${POSTGRESQL_VERSION}"
fi		
if test "x$enable_sqlite" = "xyes" ; then
  echo "       sqlite: ${SQLITE_VERSION}"
fi

echo
echo " Networking:" 
echo "   ${NETWORKING}"
echo
echo " Features: " 


echo -n "    improved ticket system: "
if test "x$enable_ticketsystemex" = "xyes" ; then
  echo "enabled"
else
  echo "disabled"
fi

echo
echo "Compiler version: "
$($CC -v)
echo
echo "LIBS=${LIBS}"
echo "CXXFLAGS=${CXXFLAGS}"
echo "LDFLAGS=${LDFLAGS}"
echo "===================================="
echo


