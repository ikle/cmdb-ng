/*
 * CMDB Match against known types
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include "cmdbs-int.h"

int cmdbs_match_type (const char *type, const char *value)
{
	if (strcmp (type, "string") == 0)
		return 1;

	/* TBD: Call external type value checker here */

	return 0;
}
