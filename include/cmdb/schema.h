/*
 * CMDB Schema
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef CMDB_SCHEMA_H
#define CMDB_SCHEMA_H  1

#include <stddef.h>

struct cmdbs *cmdbs_writer (const char *path);
struct cmdbs *cmdbs_reader (const char *path);
void cmdbs_close (struct cmdbs *o);

int cmdbs_check (struct cmdbs *o, long node, const char *value);

#endif  /* CMDB_SCHEMA_H */
