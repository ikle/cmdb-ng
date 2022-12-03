/*
 * CMDB Schema Adder
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

static long cmdbs_node_search (struct cmdbs *o, long parent, const char *label)
{
	const char *req = "SELECT id FROM tree WHERE parent = ? AND label = ?";

	if (!sqlite_compile (o->db, req, o->s + T4) ||
	    !sqlite_first (o->s[T4], "ls", parent, label))
		return 0;

	return sqlite3_column_int64 (o->s[T4], 0);
}

long cmdbs_node_add (struct cmdbs *o, long parent, int kind,
		     const char *label, const char *info)
{
	const char *req = "INSERT INTO tree (parent, kind, label, info) "
			  "VALUES (?, ?, ?, ?)";
	long id;

	if ((id = cmdbs_node_search (o, parent, label)) != 0)
		return id;

	if (!sqlite_compile (o->db, req, o->s + T1) ||
	    !sqlite_run (o->s[T1], "liss", parent, kind, label, info))
		return 0;

	return sqlite3_last_insert_rowid (o->db);
}

int cmdbs_class_add (struct cmdbs *o, long node, const char *name)
{
	const char *req = "INSERT INTO classes (node, name) VALUES (?, ?)";

	return	sqlite_compile (o->db, req, o->s + C1) &&
		sqlite_run (o->s[C1], "ls", node, name);
}

int cmdbs_class_link (struct cmdbs *o)
{
	const char *req =
		"UPDATE classes SET root = ("
		"	SELECT id FROM tree"
		"	WHERE tree.parent = 2 AND tree.label = classes.name"
		")";

	return sqlite3_exec (o->db, req, NULL, NULL, NULL) == 0;
}

int cmdbs_value_add (struct cmdbs *o, long node, const char *label,
						 const char *info)
{
	const char *req = "INSERT INTO 'values' (node, label, info) "
			  "VALUES (?, ?, ?)";

	return	sqlite_compile (o->db, req, o->s + V1) &&
		sqlite_run (o->s[V1], "lss", node, label, info);
}

int cmdbs_number_add (struct cmdbs *o, long node, long lo, long hi)
{
	const char *req = "INSERT INTO numbers (node, lo, hi) VALUES (?, ?, ?)";

	return	sqlite_compile (o->db, req, o->s + N1) &&
		sqlite_run (o->s[N1], "lll", node, lo, hi);
}

int cmdbs_match_add (struct cmdbs *o, long node, const char *name,
						 const char *re)
{
	const char *req = "INSERT INTO matches (node, name, re) "
			  "VALUES (?, ?, ?)";

	return	sqlite_compile (o->db, req, o->s + M1) &&
		sqlite_run (o->s[M1], "lss", node, name, re);
}
