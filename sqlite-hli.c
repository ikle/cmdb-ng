/*
 * SQLite High-Level Interface
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "sqlite-hli.h"

int sqlite_bind_va (sqlite3_stmt *s, const char *fmt, va_list ap)
{
	int i;

	if (sqlite3_reset (s) != 0)
		return 0;

	for (i = 1; *fmt != '\0'; ++fmt, ++i)
		switch (*fmt) {
		case 'i': {
				int x = va_arg (ap, int);

				if (sqlite3_bind_int (s, i, x) != 0)
					return 0;

				break;
			}
		case 'l': {
				long x = va_arg (ap, long);

				if (sqlite3_bind_int64 (s, i, x) != 0)
					return 0;

				break;
			}
		case 's': {
				const char *x = va_arg (ap, const char *);

				if (sqlite3_bind_text (s, i, x, -1, NULL) != 0)
					return 0;

				break;
			}
		}

	return 1;
}
