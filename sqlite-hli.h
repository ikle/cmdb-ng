/*
 * SQLite High-Level Interface
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SQLITE_HLI_H
#define SQLITE_HLI_H  1

#include <stdarg.h>
#include <stddef.h>

#include <sqlite3.h>

static inline
int sqlite_compile (sqlite3 *o, const char *req, sqlite3_stmt **s)
{
	return *s != NULL || sqlite3_prepare_v2 (o, req, -1, s, NULL) == 0;
}

int sqlite_bind_va (sqlite3_stmt *s, const char *fmt, va_list ap);

static inline int sqlite_bind (sqlite3_stmt *s, const char *fmt, ...)
{
	va_list ap;
	int ok;

	va_start (ap, fmt);
	ok = sqlite_bind_va (s, fmt, ap);
	va_end (ap);

	return ok;
}

static inline int sqlite_run (sqlite3_stmt *s, const char *fmt, ...)
{
	va_list ap;
	int ok;

	va_start (ap, fmt);
	ok = sqlite_bind_va (s, fmt, ap);
	va_end (ap);

	return ok && sqlite3_step (s) == SQLITE_DONE;
}

static inline int sqlite_first (sqlite3_stmt *s, const char *fmt, ...)
{
	va_list ap;
	int ok;

	va_start (ap, fmt);
	ok = sqlite_bind_va (s, fmt, ap);
	va_end (ap);

	return ok && sqlite3_step (s) == SQLITE_ROW;
}

static inline int sqlite_next (sqlite3_stmt *s)
{
	return sqlite3_step (s) == SQLITE_ROW;
}

#endif  /* SQLITE_HLI_H */
