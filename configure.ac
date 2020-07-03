dnl State of the Chart

AC_PREREQ(2.59)
AC_INIT([sotc], [0.1.0], [Jonas Blixt, <jonpe960@gmail.com>])

m4_pattern_forbid([^AX_(CHECK_ENABLE_DEBUG|CODE_COVERAGE|COMPILER_FLAGS|COMPILER_FLAGS_(CFLAGS|CXXFLAGS|LDFLAGS))\b],
  [Unexpanded AX_ macro found. Please install GNU autoconf-archive])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])

AM_PROG_AR
LT_INIT
AC_PROG_CC

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O

dnl Initialize Libtool

dnl Check if Libtool is present
dnl Libtool is used for building share libraries
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(JSONC, [json-c >= 0.10.0])
AC_SUBST(JSONC_CFLAGS)
AC_SUBST(JSONC_LDFLAGS)
AC_SUBST(JSONC_LIBS)

AC_CANONICAL_HOST

build_linux=no
build_windows=no
build_mac=no

# Detect the target system
case "${host_os}" in
    linux*)
        build_linux=yes
        AC_DEFINE([LINUX], 1, "Linux build")
        ;;
    cygwin*|mingw*)
        build_windows=yes
        AC_DEFINE([WINDOWS], 1, "Windows build")
        ;;
    darwin*)
        build_mac=yes
        AC_DEFINE([OSX], 1, "Mac build")
        ;;
    *)
        AC_MSG_ERROR(["OS $host_os is not supported"])
        ;;
esac

# Pass the conditionals to automake
AM_CONDITIONAL([LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([OSX], [test "$build_mac" = "yes"])

PKG_CHECK_MODULES(SDL, [sdl2 >= 2.0.0])
            AC_SUBST(SDL_CFLAGS)
            AC_SUBST(SDL_LDFLAGS)
            AC_SUBST(SDL_LIBS)

AC_CONFIG_FILES([Makefile
                lib/Makefile
                src/Makefile
                test/Makefile])


dnl Dependencies build
dnl AC_CONFIG_SUBDIRS([deps/dep1])
AX_CODE_COVERAGE
AC_OUTPUT
