/*
 * CMDB Schema Close
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include "cmdbs-int.h"

void cmdbs_close (struct cmdbs *o)
{
	int i;

	if (o == NULL)
		return;

	for (i = 0; i < ST_COUNT; ++i)
		sqlite3_finalize (o->s[i]);

	sqlite3_close_v2 (o->db);
	free (o);
}
