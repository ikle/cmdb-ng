/*
 * CMDB Schema Scan Class
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

int cmdbs_class_first (struct cmdbs_class *o, struct cmdbs *s, long node)
{
	const char *req = "SELECT name, root FROM classes "
			  "WHERE node = ? AND root IS NOT NULL";

	o->state = NULL;

	if (!sqlite_compile (s->db, req, (void *) &o->state) ||
	    !sqlite_bind (o->state, "l", node))
		return 0;

	return cmdbs_class_next (o);
}

int cmdbs_class_next (struct cmdbs_class *o)
{
	if (!sqlite_next (o->state))
		return 0;

	o->name = (void *) sqlite3_column_text (o->state, 0);
	o->root = sqlite3_column_int64         (o->state, 1);

	return 1;
}

void cmdbs_class_done (struct cmdbs_class *o)
{
	sqlite3_finalize (o->state);
}
