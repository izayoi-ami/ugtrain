/* ugpreload.c:    preloader to hook a lib into a victim process
 *
 * Copyright (c) 2013, by:      Sebastian Riemer
 *    All rights reserved.      Ernst-Reinke-Str. 23
 *                              10369 Berlin, Germany
 *                             <sebastian.riemer@gmx.de>
 *
 * This file may be used subject to the terms and conditions of the
 * GNU Library General Public License Version 2, or any later version
 * at your option, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/* env var to be added */
#define PRELOAD_VAR "LD_PRELOAD"

typedef unsigned long long u64;
typedef unsigned int u32;

int env_append(const char *name, const char *val, char separator)
{
	size_t env_len;
	const char *old_env;
	char *new_env;

	old_env = getenv(name);

	if (old_env != NULL) {
		env_len = strlen(old_env) + strlen(val) + 2;
		new_env = malloc(env_len);

		memcpy(new_env, old_env, strlen(old_env));
		memcpy(&new_env[strlen(old_env) + 1], val, strlen(val));

		new_env[strlen(old_env)] = separator;
		new_env[env_len - 1] = '\0';
	} else {
		env_len = strlen(val) + 1;
		new_env = malloc(env_len);

		memcpy(new_env, val, env_len - 1);
		new_env[env_len - 1] = '\0';
	}

	setenv(name, new_env, 1);
	free(new_env);

	return 0;
}

int main (int argc, char *argv[])
{
	char *game_path = NULL;

	if (argc < 3) {
		fprintf(stderr, "use the following parameters: "
			"<lib_path> <app_path> [app_opts]\n");
		return EXIT_FAILURE;
	}

	game_path = argv[2];

	/* append the library to the preload env var */
	env_append(PRELOAD_VAR, argv[1], ':');

	/* execute the victim code */
	if (execv(game_path, &argv[2]) < 0) {
		perror("execv");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
