/*
 * CMDB Schema Reader
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include "cmdbs-int.h"

struct cmdbs *cmdbs_reader (const char *path)
{
	const int flags = SQLITE_OPEN_READONLY;
	struct cmdbs *o;
	int i;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	if (sqlite3_open_v2 (path, &o->db, flags, NULL) != 0)
		goto no_db;

	for (i = 0; i < ST_COUNT; ++i)
		o->s[i] = NULL;

	return o;
no_db:
	free (o);
	return NULL;
}
