/*
 * Copyright (C) 2010 Davidlohr Bueso <dave@gnu.org>
 *
 * This file may be redistributed under the terms of the
 * GNU Lesser General Public License.
 *
 * General memory allocation wrappers for malloc, realloc, calloc and strdup
 */

#ifndef UTIL_LINUX_XALLOC_H
#define UTIL_LINUX_XALLOC_H

#include <stdlib.h>
#include <string.h>

// #include "c.h"
/*
 * Function attributes
 */
#ifndef __ul_alloc_size
# if (__has_attribute(alloc_size) && __has_attribute(warn_unused_result)) || __GNUC_PREREQ (4, 3)
#  define __ul_alloc_size(s) __attribute__((alloc_size(s), warn_unused_result))
# else
#  define __ul_alloc_size(s)
# endif
#endif

#ifndef __ul_calloc_size
# if (__has_attribute(alloc_size) && __has_attribute(warn_unused_result)) || __GNUC_PREREQ (4, 3)
#  define __ul_calloc_size(n, s) __attribute__((alloc_size(n, s), warn_unused_result))
# else
#  define __ul_calloc_size(n, s)
# endif
#endif

#if __has_attribute(returns_nonnull) || __GNUC_PREREQ (4, 9)
# define __ul_returns_nonnull __attribute__((returns_nonnull))
#else
# define __ul_returns_nonnull
#endif

// /*
//  * MAXHOSTNAMELEN replacement
//  */
// size_t xalloc_get_hostname_max(void)
// {
// 	long len = sysconf(_SC_HOST_NAME_MAX);
//
// 	if (0 < len)
// 		return len;
//
// #ifdef MAXHOSTNAMELEN
// 	return MAXHOSTNAMELEN;
// #elif HOST_NAME_MAX
// 	return HOST_NAME_MAX;
// #endif
// 	return 64;
// }

/*
 * xalloc.h continued
 */
#ifndef XALLOC_EXIT_CODE
# define XALLOC_EXIT_CODE EXIT_FAILURE
#endif

static inline
__attribute__((__noreturn__))
void __err_oom(const char *file, unsigned int line)
{
  fprintf(stderr, "%s: %u: cannot allocate memory\n", file, line);
  exit(XALLOC_EXIT_CODE);
}

#define err_oom()	__err_oom(__FILE__, __LINE__)

static inline
__ul_alloc_size(1)
__ul_returns_nonnull
void *xmalloc(const size_t size)
{
	void *ret = malloc(size);

	if (!ret && size) {
    fprintf(stderr, "cannot allocate %zu bytes\n", size);
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}

static inline
__ul_alloc_size(2)
__ul_returns_nonnull
void *xrealloc(void *ptr, const size_t size)
{
	void *ret = realloc(ptr, size);

	if (!ret && size) {
    fprintf(stderr, "cannot allocate %zu bytes\n", size);
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}

static inline
__ul_calloc_size(1, 2)
__ul_returns_nonnull
void *xcalloc(const size_t nelems, const size_t size)
{
	void *ret = calloc(nelems, size);

	if (!ret && size && nelems) {
    fprintf(stderr, "cannot allocate %zu bytes\n", size);
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}

static inline
__attribute__((warn_unused_result))
__ul_returns_nonnull
char *xstrdup(const char *str)
{
	char *ret;

	UK_ASSERT(str);
	ret = strdup(str);
	if (!ret) {
    fprintf(stderr, "cannot duplicate string\n");
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}

static inline
__attribute__((warn_unused_result))
__ul_returns_nonnull
char *xstrndup(const char *str, size_t size)
{
	char *ret;

	UK_ASSERT(str);
	ret = strndup(str, size);
	if (!ret) {
    fprintf(stderr, "cannot duplicate string\n");
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}


static inline
__attribute__((__format__(printf, 2, 3)))
int xasprintf(char **strp, const char *fmt, ...)
{
	int ret;
	va_list args;

	va_start(args, fmt);
	ret = vasprintf(&(*strp), fmt, args);
	va_end(args);
	if (ret < 0) {
    fprintf(stderr, "cannot allocate string\n");
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}

static inline
__attribute__((__format__(printf, 2, 0)))
int xvasprintf(char **strp, const char *fmt, va_list ap)
{
	int ret = vasprintf(&(*strp), fmt, ap);

	if (ret < 0) {
    fprintf(stderr, "cannot allocate string\n");
    exit(XALLOC_EXIT_CODE);
  }
	return ret;
}


static inline
__attribute__((warn_unused_result))
char *xgethostname(void)
{
	char *name;
	#ifdef MAXHOSTNAMELEN
		size_t sz = MAXHOSTNAMELEN + 1;
	#elif HOST_NAME_MAX
		size_t sz = HOST_NAME_MAX + 1;
	#else
		size_t sz = 64 + 1;
	#endif

	name = xmalloc(sizeof(char) * sz);
	if (gethostname(name, sz) != 0) {
		free(name);
		return NULL;
	}
	name[sz - 1] = '\0';
	return name;
}

#endif
