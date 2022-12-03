/*
 * CMDB Schema Internals
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDBS_INT_H
#define CMDBS_INT_H  1

#include <cmdb/schema-node.h>
#include <cmdb/schema-class.h>
#include <cmdb/schema-value.h>
#include <cmdb/schema-number.h>
#include <cmdb/schema-match.h>

#include "sqlite-hli.h"

enum cmdbs_stmt {
	T1 = 0, T2, T4, C1, C2,
	V1, V2, N1, N2, M1, M2,
	ST_COUNT
};

struct cmdbs {
	sqlite3 *db;
	sqlite3_stmt *s[ST_COUNT];

	struct cmdbs_node   node;
	struct cmdbs_class  c;
	struct cmdbs_value  v;
	struct cmdbs_number n;
	struct cmdbs_match  m;
};

int cmdbs_check_value  (struct cmdbs *o, long node, const char *value);
int cmdbs_check_number (struct cmdbs *o, long node, const char *value);
int cmdbs_check_match  (struct cmdbs *o, long node, const char *value);

int cmdbs_match_type (const char *type, const char *value);
int cmdbs_match_re   (const char *re,   const char *value);

#endif  /* CMDBS_INT_H */
