/*
 * A393382  a(0) = 1; a(n) = 2 * a(n-1) + a(floor(n/3)).
 * https://oeis.org/A393382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 3154 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A393382
 *  a(0) = 1,
 *  a(n) = 2 * a(n-1) + a(floor(n/3))   (n ≥ 1)
 *
 *  The function memoises computed values so that calls in any order are
 *  served rapidly.  In particular it is O(n) for the first call with
 *  argument n and O(1) thereafter, storing the results in a static
 *  dynamic array whose growth is handled by the supplied helpers.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
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
    /* static cache and its current length */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* number of entries stored   */

    /* initialise cache for a(0) */
    if (cap == 0) {
        cache = alloc_seq(1);         /* allocates an array of 1 seq_t   */
        if (cache == NULL) return 0;  /* allocation failure                */
        cache[0] = 1;                 /* a(0) = 1                         */
        cap = 1;
    }

    /* grow cache to contain a(n) if required */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        size_t old_cap = cap;
        grow_seq_array(&cache, &cap, new_cap);  /* realloc + update cap */

        /* compute all new entries up to n */
        for (size_t i = old_cap; i < cap; ++i) {
            cache[i] = (2 * cache[i-1]) + cache[i/3];
        }
    }

    return cache[(size_t)n];
}
