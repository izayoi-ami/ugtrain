/* aslr.h:    handle Address Space Layout Randomization (ASLR)
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
 *
 * By the original authors of ugtrain there shall be ABSOLUTELY
 * NO RESPONSIBILITY or LIABILITY for derived work and/or abusive
 * or malicious use. The ugtrain is an education project and
 * sticks to the copyright law by providing configs for games
 * which ALLOW CHEATING only. We can't and we don't accept abusive
 * configs or codes which might turn ugtrain into a cracker tool!
 */

#ifndef ASLR_H
#define ASLR_H

// local includes
#include <lib/list.h>
#include <lib/maps.h>
#include <fifoparser.h>
#include <cfgentry.h>
#include <common.h>
#include <options.h>

void handle_aslr (Options *opt, list<CfgEntry> *cfg, i32 ifd,
		  i32 ofd, pid_t pid, struct list_head *rlist);

void do_disc_pic_work (pid_t pid, Options *opt,
		       i32 ifd, i32 ofd, struct list_head *rlist);

void get_lib_load_addr (LF_PARAMS);

void get_stack_start (Options *opt, pid_t pid,
		      struct list_head *rlist);

void get_heap_region (Options *opt, pid_t pid,
		      struct list_head *rlist);

void get_stack_end (SF_PARAMS);

/* Inline functions */

/*
 * Helper function to get the current regions list by pid
 */
static inline void
get_regions (pid_t pid, struct list_head *rlist)
{
	struct pmap_params params;
	char exe_path[MAPS_MAX_PATH];

	get_exe_path_by_pid(pid, exe_path, sizeof(exe_path));
	params.exe_path = exe_path;
	params.rlist = rlist;
	read_regions(pid, &params);
}

// ##############################
// ###### PIC/PIE handling ######
// ##############################

/*
 * Handle PIE for static memory
 */
static inline void
handle_statmem_pie (Options *opt, list<CfgEntry> *cfg)
{
	list<CfgEntry>::iterator it;
	list<CheckEntry> *chk_lp;
	list<CheckEntry>::iterator chk_it;
	ptr_t code_offs = opt->code_offs;

	list_for_each (cfg, it) {
		if (it->dynmem || it->ptrmem)
			continue;
		if (!it->type.on_stack) {
			it->addr += code_offs;
			// change the cache address only once
			if (!it->cache->is_dirty) {
				it->cache->is_dirty = true;
				it->cache->offs += code_offs;
			}
		}
		if (!it->checks)
			continue;
		chk_lp = it->checks;
		list_for_each (chk_lp, chk_it) {
			if (chk_it->type.on_stack)
				continue;
			chk_it->addr += code_offs;
			// change the cache address only once
			if (!chk_it->cache->is_dirty) {
				chk_it->cache->is_dirty = true;
				chk_it->cache->offs += code_offs;
			}
		}
	}

	// clear dirty flags again
	list<CacheEntry>::iterator cait;
	list_for_each (opt->cache_list, cait)
		cait->is_dirty = false;
}

#endif
