/*
 * CMDB Schema Value
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDB_SCHEMA_VALUE_H
#define CMDB_SCHEMA_VALUE_H  1

#include <cmdb/schema.h>

struct cmdbs_value {
	void *state;

	const char *label, *info;
};

int cmdbs_value_add (struct cmdbs *o, long node, const char *label,
						 const char *info);

int  cmdbs_value_first (struct cmdbs_value *o, struct cmdbs *s, long node);
int  cmdbs_value_next  (struct cmdbs_value *o);
void cmdbs_value_done  (struct cmdbs_value *o);

#endif  /* CMDB_SCHEMA_VALUE_H */
