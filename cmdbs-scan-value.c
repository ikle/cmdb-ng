/*
 * CMDB Schema Scan Value
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

int cmdbs_value_first (struct cmdbs_value *o, struct cmdbs *s, long node)
{
	const char *req = "SELECT label, info FROM 'values' WHERE node = ?";

	o->state = NULL;

	if (!sqlite_compile (s->db, req, (void *) &o->state) ||
	    !sqlite_bind (o->state, "l", node))
		return 0;

	return cmdbs_value_next (o);
}

int cmdbs_value_next (struct cmdbs_value *o)
{
	if (!sqlite_next (o->state))
		return 0;

	o->label = (void *) sqlite3_column_text (o->state, 0);
	o->info  = (void *) sqlite3_column_text (o->state, 1);

	return 1;
}

void cmdbs_value_done (struct cmdbs_value *o)
{
	sqlite3_finalize (o->state);
}
