/*
 * CMDB Match against regular expression
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <regex.h>

#include "cmdbs-int.h"

int cmdbs_match_re (const char *re, const char *value)
{
	regex_t c;
	int ok;

	if (re == NULL)
		return 1;

	if (regcomp (&c, re, REG_EXTENDED | REG_NOSUB) != 0)
		return 0;

	ok = regexec (&c, value, 0, NULL, 0) == 0;

	regfree (&c);
	return ok;
}
