/*
 * CMDB Schema Match
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDB_SCHEMA_MATCH_H
#define CMDB_SCHEMA_MATCH_H  1

#include <cmdb/schema.h>

struct cmdbs_match {
	void *state;

	const char *name, *re;
};

int cmdbs_match_add (struct cmdbs *o, long node, const char *name,
						 const char *re);

int  cmdbs_match_first (struct cmdbs_match *o, struct cmdbs *s, long node);
int  cmdbs_match_next  (struct cmdbs_match *o);
void cmdbs_match_done  (struct cmdbs_match *o);

#endif  /* CMDB_SCHEMA_MATCH_H */
