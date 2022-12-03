/*
 * CMDB Schema Scan Tree
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

int cmdbs_node_first (struct cmdbs_node *o, struct cmdbs *s, long node)
{
	const char *req = "SELECT * FROM tree "
			  "WHERE parent = ? ORDER BY label ASC";

	o->state = NULL;

	if (!sqlite_compile (s->db, req, (void *) &o->state) ||
	    !sqlite_bind (o->state, "l", node))
		return 0;

	return cmdbs_node_next (o);
}

int cmdbs_node_next (struct cmdbs_node *o)
{
	if (!sqlite_next (o->state))
		return 0;

	o->id     = sqlite3_column_int64         (o->state, 0);
	o->parent = sqlite3_column_int64         (o->state, 1);
	o->kind   = sqlite3_column_int           (o->state, 2);
	o->label  = (void *) sqlite3_column_text (o->state, 3);
	o->info   = (void *) sqlite3_column_text (o->state, 4);

	return 1;
}

void cmdbs_node_done (struct cmdbs_node *o)
{
	sqlite3_finalize (o->state);
}
