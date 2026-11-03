/*
 * A266046  Real part of Q^n, where Q is the quaternion 2 + j + k.
 * https://oeis.org/A266046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 24, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 921 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* allocated array of a(k) values */
    static size_t cache_len = 0;         /* number of elements actually stored */

    /* Handle the two base cases explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* Initialise the cache if this is the first call */
    if (cache == NULL) {
        cache = alloc_seq_array(2);      /* allocate space for a(0) and a(1) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }

    /* If the requested n is beyond what we have computed, extend the cache */
    if ((idx_t)cache_len <= n) {
        /* Remember the old size to know from where to start the recurrence */
        size_t old_len = cache_len;

        /* Grow the array to accommodate indices up to n */
        grow_seq_array(&cache, &cache_len, (size_t)(n + 1));

        /* Compute missing terms using the linear recurrence */
        for (idx_t i = old_len; i <= n; ++i) {
            /* a(i) = 4*a(i-1) - 6*a(i-2) */
            cache[i] = 4 * cache[i - 1] - 6 * cache[i - 2];
        }

        /* Update the logical length of the cache */
        cache_len = (size_t)(n + 1);
    }

    return cache[n];
}
