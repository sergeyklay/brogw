/* matcher.h
   Copyright (C) 2024 Free Software Foundation, Inc.

This file is part of Brogw.

Brogw is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Brogw is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Brigw.  If not, see <https://www.gnu.org/licenses/>.  */


#ifndef BROGW_MATCHER_H
#define BROGW_MATCHER_H

#include "brogw_config.h"

/* Find a matching browser command for the given URL using the list of rules.
   If no rule matches, return default_browser. */
const char *matcher_find_command(const char *url, const Rule *rules,
                                 size_t cnt, const char *default_browser);

#endif
