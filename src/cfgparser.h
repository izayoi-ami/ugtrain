/* cfgparser.h:    parsing functions to read in the config file
 *
 * Copyright (c) 2012..2018 Sebastian Parschauer <s.parschauer@gmx.de>
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

// local includes
#include <cfgentry.h>
#include <common.h>
#include <options.h>

void read_config (Options *opt, vector<string> *cfg_lines);

void write_config_vect (char *path, vector<string> *cfg_lines);
