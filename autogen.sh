#!/bin/sh
# Brogw Build Bootstrap Script
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

set -e

# Ensure we are in the project root
srcdir=$(dirname "$0")
test -n "$srcdir" || srcdir=.

echo "Preparing the build system..."

if ! command -v autoreconf > /dev/null 2>&1; then
  echo "**Error**: autoreconf is required to bootstrap this project."
  exit 1
fi

echo "Running autoreconf..."
autoreconf --install --force --verbose

echo "Cleaning up temporary files..."
rm -rf autom4te.cache

echo "Done. Now you can run './configure' and 'make' to build the project."
