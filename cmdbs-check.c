/*
 * CMDB Schema Check
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "cmdbs-int.h"

int cmdbs_check (struct cmdbs *o, long node, const char *value)
{
	return	cmdbs_check_value  (o, node, value) ||
		cmdbs_check_number (o, node, value) ||
		cmdbs_check_match  (o, node, value);
}
