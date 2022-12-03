/*
 * CMDB Schema Scan Match
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

int cmdbs_match_first (struct cmdbs_match *o, struct cmdbs *s, long node)
{
	const char *req = "SELECT name, re FROM matches WHERE node = ?";

	o->state = NULL;

	if (!sqlite_compile (s->db, req, (void *) &o->state) ||
	    !sqlite_bind (o->state, "l", node))
		return 0;

	return cmdbs_match_next (o);
}

int cmdbs_match_next (struct cmdbs_match *o)
{
	if (!sqlite_next (o->state))
		return 0;

	o->name = (void *) sqlite3_column_text (o->state, 0);
	o->re   = (void *) sqlite3_column_text (o->state, 1);

	return 1;
}

void cmdbs_match_done (struct cmdbs_match *o)
{
	sqlite3_finalize (o->state);
}
