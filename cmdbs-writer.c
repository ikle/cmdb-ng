/*
 * CMDB Schema Writer
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include "cmdbs-int.h"

static const char *tree_init =
	"CREATE TABLE IF NOT EXISTS tree (\n"
	"	id	INTEGER PRIMARY KEY,\n"	/* id > 0		*/
	"	parent	INTEGER NOT NULL,\n"	/* 0 for root		*/
	"	kind	INTEGER NOT NULL,\n"
	"	label	TEXT    NOT NULL,\n"
	"	info	TEXT    NOT NULL,\n"	/* short description	*/

	"	UNIQUE (parent, label)\n"
	");\n"

	"CREATE INDEX IF NOT EXISTS 'tree-parent' ON tree (parent);\n"

	"INSERT OR IGNORE INTO tree (id, parent, kind, label, info)\n"
	"VALUES (1, 0, 0, 'root', 'Configuration Template');\n"

	"INSERT OR IGNORE INTO tree (id, parent, kind, label, info)\n"
	"VALUES (2, 0, 0, 'class', 'Configuration Classes');\n";

static const char *class_init =
	"CREATE TABLE IF NOT EXISTS classes (\n"
	"	node	INTEGER NOT NULL,\n"
	"	name	TEXT NOT NULL,\n"
	"	root	INTEGER,\n"	/* tree id for present classes */

	"	UNIQUE (node, name)\n"
	");\n"

	"CREATE INDEX IF NOT EXISTS 'classes-node' ON classes (node);\n";

static const char *value_init =
	"CREATE TABLE IF NOT EXISTS 'values' (\n"
	"	node	INTEGER NOT NULL,\n"
	"	label	TEXT    NOT NULL,\n"
	"	info	TEXT    NOT NULL,\n"

	"	UNIQUE (node, label)\n"
	");\n"

	"CREATE INDEX IF NOT EXISTS 'values-node' ON values (node);\n";

static const char *number_init =
	"CREATE TABLE IF NOT EXISTS numbers (\n"
	"	node	INTEGER NOT NULL,\n"
	"	lo	INTEGER NOT NULL,\n"
	"	hi	INTEGER NOT NULL,\n"

	"	UNIQUE (node, lo, hi)\n"
	");\n"

	"CREATE INDEX IF NOT EXISTS 'numbers-node' ON numbers (node);\n";

static const char *match_init =
	"CREATE TABLE IF NOT EXISTS matches (\n"
	"	node	INTEGER NOT NULL,\n"
	"	name	TEXT NOT NULL,\n"
	"	re	TEXT,\n"

	"	UNIQUE (node, name)\n"
	");\n"

	"CREATE INDEX IF NOT EXISTS 'matches-node' ON matches (node);\n";

static int cmdbs_init (struct cmdbs *o)
{
	return	sqlite3_exec (o->db, tree_init,   NULL, NULL, NULL) == 0 &&
		sqlite3_exec (o->db, class_init,  NULL, NULL, NULL) == 0 &&
		sqlite3_exec (o->db, value_init,  NULL, NULL, NULL) == 0 &&
		sqlite3_exec (o->db, number_init, NULL, NULL, NULL) == 0 &&
		sqlite3_exec (o->db, match_init,  NULL, NULL, NULL) == 0;
}

struct cmdbs *cmdbs_writer (const char *path)
{
	const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	struct cmdbs *o;
	int i;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	if (sqlite3_open_v2 (path, &o->db, flags, NULL) != 0)
		goto no_db;

	for (i = 0; i < ST_COUNT; ++i)
		o->s[i] = NULL;

	if (!cmdbs_init (o))
		goto no_init;

	return o;
no_init:
	sqlite3_close_v2 (o->db);
no_db:
	free (o);
	return NULL;
}
