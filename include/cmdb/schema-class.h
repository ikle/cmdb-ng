/*
 * CMDB Schema Class
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDB_SCHEMA_CLASS_H
#define CMDB_SCHEMA_CLASS_H  1

#include <cmdb/schema.h>

struct cmdbs_class {
	void *state;

	const char *name;
	long root;
};

int cmdbs_class_add  (struct cmdbs *o, long node, const char *name);
int cmdbs_class_link (struct cmdbs *o);

int  cmdbs_class_first (struct cmdbs_class *o, struct cmdbs *s, long node);
int  cmdbs_class_next  (struct cmdbs_class *o);
void cmdbs_class_done  (struct cmdbs_class *o);

#endif  /* CMDB_SCHEMA_CLASS_H */
