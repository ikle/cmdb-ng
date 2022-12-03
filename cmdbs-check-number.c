/*
 * CMDB Schema Check Number
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include "cmdbs-int.h"

static int cmdbs_check_number_nr (struct cmdbs *o, long node, long x)
{
	const char *req = "SELECT COUNT (1) FROM numbers"
			  "WHERE node = ? AND lo >= ? AND ? <= hi LIMIT 1";

	if (!sqlite_compile (o->db, req, o->s + N2) ||
	    !sqlite_first (o->s[N2], "lll", node, x, x))
		return 0;

	return sqlite3_column_int (o->s[N2], 0);
}

int cmdbs_check_number (struct cmdbs *o, long node, const char *value)
{
	char *end;
	long x = strtol (value, &end, 0);
	struct cmdbs_class c;
	int ok;

	if (*end != '\0')
		return 0;

	if (cmdbs_check_number_nr (o, node, x))
		return 1;

	for (
		ok = cmdbs_class_first (&c, o, node);
		ok;
		ok = cmdbs_class_next (&c)
	)
		if (cmdbs_check_number_nr (o, c.root, x)) {
			cmdbs_class_done (&c);
			return 1;
		}

	cmdbs_class_done (&c);
	return 0;
}
