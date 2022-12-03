/*
 * CMDB Schema Lookup
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

static struct cmdbs_node *
cmdbs_node_lookup_nr (struct cmdbs *o, long parent, const char *label)
{
	const char *req = "SELECT * FROM tree WHERE parent = ? AND label = ?";
	struct cmdbs_node *n = &o->node;

	if (!sqlite_compile (o->db, req, o->s + T2) ||
	    !sqlite_first (o->s[T2], "ls", parent, label))
		return NULL;

	n->id     = sqlite3_column_int64         (o->s[T2], 0);
	n->parent = sqlite3_column_int64         (o->s[T2], 1);
	n->kind   = sqlite3_column_int           (o->s[T2], 2);
	n->label  = (void *) sqlite3_column_text (o->s[T2], 3);
	n->info   = (void *) sqlite3_column_text (o->s[T2], 4);

	return n;
}

struct cmdbs_node *
cmdbs_node_lookup (struct cmdbs *o, long parent, const char *label)
{
	struct cmdbs_node *n;
	struct cmdbs_class c;
	int ok;

	if ((n = cmdbs_node_lookup_nr (o, parent, label)) != NULL)
		return n;

	for (
		ok = cmdbs_class_first (&c, o, parent);
		ok;
		ok = cmdbs_class_next (&c)
	)
		if ((n = cmdbs_node_lookup (o, c.root, label)) != NULL) {
			cmdbs_class_done (&c);
			return n;
		}

	cmdbs_class_done (&c);
	return NULL;
}
