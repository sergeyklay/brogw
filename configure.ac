# Copyright (C) 2024 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <https://www.gnu.org/licenses/>.

AC_INIT([Browser Router], [1.0], [gnu@serghei.pl])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries
AC_CHECK_HEADERS([regex.h stdlib.h string.h unistd.h])

# Output files
AC_CONFIG_FILES([Makefile src/Makefile etc/Makefile])
AC_OUTPUT
