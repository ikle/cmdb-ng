/*
 * CMDB Schema Node
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDB_SCHEMA_NODE_H
#define CMDB_SCHEMA_NODE_H  1

#include <cmdb/schema.h>

enum cmdbs_kind {
	CMDBS_GROUP	= 0,
	CMDBS_NODE,
	CMDBS_NODES,
	CMDBS_ATTR,
	CMDBS_ATTRS,
};

struct cmdbs_node {
	void *state;

	long id, parent;
	const char *label, *info;
	unsigned char kind;
};

long cmdbs_node_add (struct cmdbs *o, long parent, int kind,
		     const char *label, const char *info);

struct cmdbs_node *
cmdbs_node_lookup (struct cmdbs *o, long parent, const char *label);

int  cmdbs_node_first (struct cmdbs_node *o, struct cmdbs *s, long parent);
int  cmdbs_node_next  (struct cmdbs_node *o);
void cmdbs_node_done  (struct cmdbs_node *o);

#endif  /* CMDB_SCHEMA_NODE_H */
