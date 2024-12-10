dnl Copyright (C) 2024 Free Software Foundation, Inc.
dnl
dnl This file is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU General Public License
dnl as published by the Free Software Foundation; either version 3
dnl of the License, or (at your option) any later version.
dnl
dnl This file is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this file.  If not, see <https://www.gnu.org/licenses/>.

dnl Process this file with autoconf to produce a configure script.

dnl Prelude.
dnl -------------------------------------------------------------------------
AC_PREREQ(2.53)
AC_INIT([Browser Router], [1.0], [gnu@serghei.pl])

dnl The place to put some extra build scripts installed.
dnl -------------------------------------------------------------------------
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries
AC_CHECK_HEADERS([regex.h stdlib.h string.h unistd.h])

AC_LANG([C])
AC_CANONICAL_HOST
AC_CANONICAL_BUILD

BUILD_OS=$build_os
AC_SUBST(BUILD_OS)

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

dnl Fairly severe build strictness
dnl -------------------------------------------------------------------------
AM_INIT_AUTOMAKE([foreign])

dnl Make the compilation flags quiet unless V=1 is used
dnl -------------------------------------------------------------------------
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Check if the CFLAGS has been overridden (here before libtool and ar).
if test "x${CFLAGS+set}" = "xset"; then
  cflags_overridden=yes
else
  cflags_overridden=no
fi

dnl Enable debug
dnl --------------------------------------------------------------------------
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug],
                  [use debug compiler flags and macros (default is no)])],
  [enable_debug=$enableval],
  [enable_debug=no]
)

dnl Turn warnings into errors
dnl --------------------------------------------------------------------------
AC_ARG_ENABLE([werror],
  [AS_HELP_STRING([--enable-werror],
                  [treat certain compiler warnings as errors (default is no)])],
  [enable_werror=$enableval],
  [enable_werror=no]
)
AX_CHECK_COMPILE_FLAG([-Werror], [CFLAG_WERROR="-Werror"], [CFLAG_WERROR=""])

if test "x$enable_debug" = xyes; then
  # Clear default -g -O2 flags
  if test "x$cflags_overridden" = xno; then
    CFLAGS=""
  fi

  AC_DEFINE(BROGW_DEBUG, 1, [Whether debug is enabled])

  # Prefer -Og, fall back to -O0 if that is unavailable.
  AX_CHECK_COMPILE_FLAG(
    [-Og],
    [[DEBUG_CFLAGS="$DEBUG_CFLAGS -Og"]],
    [AX_CHECK_COMPILE_FLAG([-O0],[[DEBUG_CFLAGS="$DEBUG_CFLAGS -O0"]],,[[$CFLAG_WERROR]])],
    [[$CFLAG_WERROR]])

  # Prefer -g3, fall back to -g if that is unavailable.
  AX_CHECK_COMPILE_FLAG(
    [-g3],
    [[DEBUG_CFLAGS="$DEBUG_CFLAGS -g3"]],
    [AX_CHECK_COMPILE_FLAG([-g],[[DEBUG_CFLAGS="$DEBUG_CFLAGS -g"]],,[[$DEBUG_CFLAGS]])],
    [[$CFLAG_WERROR]])

  AX_CHECK_PREPROC_FLAG([-DDEBUG],[[DEBUG_CFLAGS="$DEBUG_CFLAGS -DDEBUG"]],,[[$DEBUG_CFLAGS]])
else
  AC_DEFINE(BROGW_DEBUG, 0, [Whether debug is enabled])
fi


ERROR_CFLAGS=
if test "x$enable_werror" = "xyes"; then
  if test "x$CFLAG_WERROR" = "x"; then
     AC_MSG_ERROR("enable-werror set but -Werror is not usable")
  fi
  AX_CHECK_COMPILE_FLAG([-Werror=vla],[ERROR_CFLAGS="$ERROR_CFLAGS -Werror=vla"],,[[$CFLAG_WERROR]])
fi

if test "x$cflags_overridden" = "xno"; then
  AX_CHECK_COMPILE_FLAG([-Wall],[WARN_CFLAGS="$WARN_CFLAGS -Wall"],,[[$CFLAG_WERROR]])
  AX_CHECK_COMPILE_FLAG([-Wvla],[WARN_CFLAGS="$WARN_CFLAGS -Wvla"],,[[$CFLAG_WERROR]])

  ## Some compilers (gcc) ignore unknown -Wno-* options, but warn about all
  ## unknown options if any other warning is produced. Test the -Wfoo case, and
  ## set the -Wno-foo case if it works.
  AX_CHECK_COMPILE_FLAG([-Wunused-variable],[NOWARN_CFLAGS="$NOWARN_CFLAGS -Wno-unused-variable"],,[[$CFLAG_WERROR]])
fi

case $host in
  *mingw*)
    TARGET_OS=windows
    ;;
  *darwin*)
    TARGET_OS=darwin
    ;;
  *linux*)
    TARGET_OS=linux
    ;;
  *)
    AC_MSG_ERROR(Unsupported host: $host. Please file a bug report.)
    ;;
esac

dnl this flag screws up non-darwin gcc even when the check fails. special-case it.
if test x$TARGET_OS = xdarwin; then
  AX_CHECK_LINK_FLAG([[-Wl,-dead_strip]], [LDFLAGS="$LDFLAGS -Wl,-dead_strip"])
fi

AM_CONDITIONAL([TARGET_DARWIN], [test "x$TARGET_OS" = "xdarwin"])
AM_CONDITIONAL([TARGET_WINDOWS], [test "x$TARGET_OS" = "xwindows"])
AM_CONDITIONAL([TARGET_LINUX], [test "x$TARGET_OS" = "xlinux"])

# Output files
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  etc/Makefile
])

AC_SUBST(DEBUG_CFLAGS)
AC_SUBST(WARN_CFLAGS)
AC_SUBST(NOWARN_CFLAGS)
AC_SUBST(ERROR_CFLAGS)

AC_OUTPUT

echo
echo "Options used to compile and link:"
echo "  debug enabled = $enable_debug"
echo "  target os     = $TARGET_OS"
echo "  build os      = $BUILD_OS"
echo
echo "  CC            = $CC"
echo "  CFLAGS        = $DEBUG_CFLAGS $WARN_CFLAGS $NOWARN_CFLAGS $ERROR_CFLAGS $CFLAGS"
echo
