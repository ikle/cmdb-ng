/*
 * CMDB Schema Number
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDB_SCHEMA_NUMBER_H
#define CMDB_SCHEMA_NUMBER_H  1

#include <cmdb/schema.h>

struct cmdbs_number {
	void *state;

	long lo, hi;
};

int cmdbs_number_add (struct cmdbs *o, long node, long lo, long hi);

int  cmdbs_number_first (struct cmdbs_number *o, struct cmdbs *s, long node);
int  cmdbs_number_next  (struct cmdbs_number *o);
void cmdbs_number_done  (struct cmdbs_number *o);

#endif  /* CMDB_SCHEMA_NUMBER_H */
