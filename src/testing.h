/* testing.h:    inline functions to check if ugtrain does it right
 *
 * Copyright (c) 2014..2018 Sebastian Parschauer <s.parschauer@gmx.de>
 *
 * This file may be used subject to the terms and conditions of the
 * GNU General Public License Version 3, or any later version
 * at your option, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include <list>
#include <stdlib.h>

/* local includes */
#include <cfgentry.h>
#include <options.h>
#include <testopts.h>

#ifdef TESTING

static inline void test_optparsing (Options *opt)
{
	if (opt->exit_after != PART_OPT_PARSING)
		return;
	exit(0);
}

static inline void test_cfgparsing (Options *opt)
{
	if (opt->exit_after != PART_CFG_PARSING)
		return;
	exit(0);
}

static inline void test_cfgoutput (Options *opt)
{
	if (opt->exit_after != PART_CFG_OUTPUT)
		return;
	exit(0);
}

#else

static inline void test_optparsing (Options *opt) {}
static inline void test_cfgparsing (Options *opt) {}
static inline void test_cfgoutput (Options *opt) {}

#endif
