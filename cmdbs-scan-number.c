/*
 * CMDB Schema Scan Number
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

int cmdbs_number_first (struct cmdbs_number *o, struct cmdbs *s, long node)
{
	const char *req = "SELECT lo, hi FROM numbers WHERE node = ?";

	o->state = NULL;

	if (!sqlite_compile (s->db, req, (void *) &o->state) ||
	    !sqlite_bind (o->state, "l", node))
		return 0;

	return cmdbs_number_next (o);
}

int cmdbs_number_next (struct cmdbs_number *o)
{
	if (!sqlite_next (o->state))
		return 0;

	o->lo = sqlite3_column_int64 (o->state, 0);
	o->hi = sqlite3_column_int64 (o->state, 1);

	return 1;
}

void cmdbs_number_done (struct cmdbs_number *o)
{
	sqlite3_finalize (o->state);
}
