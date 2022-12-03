/*
 * CMDB Schema Check Match
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

static void match_cb (sqlite3_context *o, int argc, sqlite3_value **argv)
{
	const char *value = (void *) sqlite3_value_text (argv[0]);
	const char *name  = (void *) sqlite3_value_text (argv[1]);
	const char *re    = (void *) sqlite3_value_text (argv[2]);

	int ok = cmdbs_match_type (name, value);

	if (ok && re != NULL)
		ok = cmdbs_match_re (re, value);

	sqlite3_result_int (o, ok);
}

static int cmdbs_check_match_nr (struct cmdbs *o, long node, const char *value)
{
	const char *req = "SELECT COUNT (1) FROM matchers"
			  "WHERE node = ? AND match (?, name, re) LIMIT 1";

	if (!sqlite_compile (o->db, req, o->s + M2) ||
	    !sqlite_first (o->s[M2], "ls", node, value))
		return 0;

	return sqlite3_column_int (o->s[M2], 0);
}

int cmdbs_match_value (struct cmdbs *o, long node, const char *value)
{
	const int rep = SQLITE_UTF8 | SQLITE_DETERMINISTIC;
	struct cmdbs_class c;
	int ok;

	if (o->s[M2] == NULL &&
	    sqlite3_create_function (o->db, "match", 3, rep, NULL,
				     match_cb, NULL, NULL) != 0)
		return 0;

	if (cmdbs_check_match_nr (o, node, value))
		return 1;

	for (
		ok = cmdbs_class_first (&c, o, node);
		ok;
		ok = cmdbs_class_next (&c)
	)
		if (cmdbs_check_match_nr (o, c.root, value)) {
			cmdbs_class_done (&c);
			return 1;
		}

	cmdbs_class_done (&c);
	return 0;
}
