/* system.h:    provide OS specific helpers (e.g. run cmds)
 *
 * Copyright (c) 2012..14, by:  Sebastian Parschauer
 *    All rights reserved.     <s.parschauer@gmx.de>
 *
 * powered by the Open Game Cheating Association
 *
 * This file may be used subject to the terms and conditions of the
 * GNU General Public License Version 2, or any later version
 * at your option, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>
#include "common.h"
#ifdef __linux__
#include <sys/wait.h>
#include <sys/select.h>
#else
#include <windows.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif
	char    *get_abs_app_path (char *app_name);
	pid_t   proc_to_pid (char *proc_name);
	ssize_t run_cmd_pipe (const char *cmd, char *const cmdv[],
			      char *pbuf, size_t pbuf_size);
	pid_t   run_cmd_bg (const char *cmd, char *const cmdv[],
			    bool do_wait, char *preload_lib);
	pid_t   run_pgrp_bg (const char *pcmd, char *const pcmdv[],
			     const char *ccmd, char *const ccmdv[],
			     char *const pid_cmd, char *proc_name,
			     u32 delay, bool do_wait, char *preload_lib);
	bool    pid_is_running (pid_t call_pid, pid_t pid,
				char *proc_name, bool use_wait);
	pid_t   fork_proc (void (*task) (void *), void *argp);
	void    *get_code_offs (pid_t pid, char *proc_name);
#ifdef __cplusplus
};
#endif

/* Inline functions */
static inline pid_t run_cmd (const char *cmd, char *const cmdv[])
{
	return run_cmd_bg(cmd, cmdv, true, NULL);
}

#ifdef __linux__
static inline i32 rm_file (const char *path)
{
	return unlink(path);
}

static inline void sleep_sec (u32 sec)
{
	sleep(sec);
}

static inline void sleep_sec_unless_input (u32 sec)
{
	fd_set fs;
	struct timeval tv;

	FD_ZERO(&fs);
	FD_SET(STDIN_FILENO, &fs);
	tv.tv_sec = sec;
	tv.tv_usec = 0;

	select(1, &fs, NULL, NULL, &tv);
}

static inline void wait_proc (pid_t pid)
{
	i32 status;

	waitpid(pid, &status, 0);
}

static inline void kill_proc (pid_t pid)
{
	kill(pid, SIGINT);
}

#else

static inline i32 rm_file (const char *path)
{
}

static inline void sleep_sec (u32 sec)
{
	Sleep(sec * 1000);
}

static inline void sleep_sec_unless_input (u32 sec)
{
	sleep_sec(sec);
}

static inline void wait_proc (pid_t pid)
{
}

static inline void kill_proc (pid_t pid)
{
}

#endif

#endif
