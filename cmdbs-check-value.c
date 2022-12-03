/*
 * CMDB Schema Check Value
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

static int cmdbs_check_value_nr (struct cmdbs *o, long node, const char *value)
{
	const char *req = "SELECT COUNT (1) FROM 'values'"
			  "WHERE node = ? AND label = ? LIMIT 1";

	if (!sqlite_compile (o->db, req, o->s + V2) ||
	    !sqlite_first (o->s[V2], "ls", node, value))
		return 0;

	return sqlite3_column_int (o->s[V2], 0);
}

int cmdbs_check_value (struct cmdbs *o, long node, const char *value)
{
	struct cmdbs_class c;
	int ok;

	if (cmdbs_check_value_nr (o, node, value))
		return 1;

	for (
		ok= cmdbs_class_first (&c, o, node);
		ok;
		ok = cmdbs_class_next (&c)
	)
		if (cmdbs_check_value_nr (o, c.root, value)) {
			cmdbs_class_done (&c);
			return 1;
		}

	cmdbs_class_done (&c);
	return 0;
}
