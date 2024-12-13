dnl Autoconf script for Brogw.
dnl To rebuild the 'configure' script from this, execute the command
dnl     autoconf
dnl in directory containing this script.
dnl If you changed any AC_DEFINEs, also run autoheader.
dnl
dnl Copyright (C) 2024 Free Software Foundation, Inc.
dnl
dnl This file is part of Brogw.
dnl
dnl Bgrogw is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl Brogw is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with Brogw.  If not, see <https://www.gnu.org/licenses/>.

dnl Prelude.
dnl -------------------------------------------------------------------------
AC_PREREQ(2.53)
AC_INIT([Brogw], [1.0], [gnu@serghei.pl], [],
  [https://github.com/sergeyklay/brogw])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])

dnl Fairly severe build strictness
dnl -------------------------------------------------------------------------
AM_INIT_AUTOMAKE([foreign])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL

# Make sure we have POSIX regex ability.
AC_CHECK_HEADER([regex.h],, AC_MSG_ERROR([POSIX regex.h is required]))
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

AC_LANG([C])

dnl Canonicalize the configuration name.
AC_CANONICAL_HOST
AC_CANONICAL_BUILD

dnl -lregex is needed for Windows only.
HAVE_LIBREGEX=no
case $host in
  *mingw*)
    AC_CHECK_LIB([regex], [regfree],
      [HAVE_LIBREGEX=yes], AC_MSG_ERROR([regex libray not found]))
    if test "${HAVE_LIBREGEX}" = "yes"; then
      LIBS="$LIBS -lregex"
    fi
    ;;
  *darwin*)
    ;;
  *linux*)
    ;;
  *)
    AC_MSG_ERROR(Unsupported host: $host. Please file a bug report.)
    ;;
esac

# Output files
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  etc/Makefile
])

AC_OUTPUT

AS_ECHO(["
Configured for '${host}'.

  Where should the build process find the source code?    ${srcdir}
  What compiler should brogw be built with?               ${CC} ${CFLAGS}
"])

AS_ECHO(["  Does Brogw use -lregex?                                 ${HAVE_LIBREGEX}
"])
